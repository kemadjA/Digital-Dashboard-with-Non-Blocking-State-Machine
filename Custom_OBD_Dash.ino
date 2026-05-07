#include <WiFi.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "ui.h" // The file exported from SquareLine

// ==========================================
// CONFIGURATION
// ==========================================
const char* ssid = "WiFi_OBDII";
const char* password = "";
const char* host = "192.168.0.10";
const uint16_t port = 35000;

// ==========================================
// GLOBALS
// ==========================================
TFT_eSPI tft = TFT_eSPI(); 
WiFiClient client;

int currentRPM = 0;
int currentSpeed = 0;
int currentTemp = 0;
int currentBoostKpa = 0;

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 200; 
int obdState = 0; 
bool elmInitialized = false;
String responseBuffer = "";

// ==========================================
// LVGL DISPLAY DRIVER (The Glue)
// ==========================================
// This screen buffer is required for LVGL to draw graphics quickly
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; // Buffer for 10 rows of the screen

void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  
  // 1. Init TFT
  tft.init();
  tft.setRotation(3); 
  
  // 2. Init LVGL
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // 3. Load SquareLine UI
  ui_init(); 

  // 4. Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // 3. Connect TCP Client & Init ELM327
  connectToELM();
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  // Let LVGL handle animations and UI drawing
  lv_timer_handler();
  delay(5);

  // Handle Wi-Fi & TCP Connection Non-Blocking
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    reconnect();
    return;
  }

  // Read ELM327 Data
  while (client.connected() && client.available()) {
    char c = client.read();
    responseBuffer += c;
    if (c == '>') {
      parseOBDResponse(responseBuffer);
      responseBuffer = ""; 
    }
  }

  // Request Data
  if (elmInitialized && (millis() - lastRequestTime >= requestInterval)) {
    lastRequestTime = millis();
    requestNextPID();
  }
}

// ==========================================
// ELM327 & OBD LOGIC
// ==========================================
void connectToELM() {
  if (client.connect(host, port)) {
    client.print("ATZ\r"); delay(1000); 
    client.print("ATE0\r"); delay(200);
    client.print("ATL0\r"); delay(200);
    client.print("ATSP4\r"); delay(500);
    client.print("0100\r"); delay(2500); 
    elmInitialized = true;
  }
}

void reconnect() {
  elmInitialized = false;
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(1000);
  } else {
    connectToELM();
    delay(1000);
  }
}

void requestNextPID() {
  switch (obdState) {
    case 0: client.print("010C\r"); break; // RPM
    case 1: client.print("010D\r"); break; // Speed
    case 2: client.print("0105\r"); break; // Temp
    case 3: client.print("010B\r"); break; // MAP (Boost)
  }
  obdState++;
  if (obdState > 3) obdState = 0;
}

void parseOBDResponse(String response) {
  response.replace(" ", "");
  response.replace("\r", "");
  response.replace("\n", "");
  response.replace(">", "");

  if (response.startsWith("41")) {
    String pid = response.substring(2, 4);
    
    if (pid == "0C" && response.length() >= 8) {
      long A = strtol(response.substring(4, 6).c_str(), NULL, 16);
      long B = strtol(response.substring(6, 8).c_str(), NULL, 16);
      currentRPM = ((A * 256) + B) / 4;
      updateUI();
    }
    else if (pid == "0D" && response.length() >= 6) {
      currentSpeed = strtol(response.substring(4, 6).c_str(), NULL, 16);
      updateUI();
    }
    else if (pid == "05" && response.length() >= 6) {
      currentTemp = strtol(response.substring(4, 6).c_str(), NULL, 16) - 40;
      updateUI();
    }
    else if (pid == "0B" && response.length() >= 6) {
      currentBoostKpa = strtol(response.substring(4, 6).c_str(), NULL, 16) - 100;
      if (currentBoostKpa < 0) currentBoostKpa = 0;
      updateUI();
    }
  }
}
// ==========================================
// SQUARELINE UI UPDATE LOGIC
// ==========================================
void updateUI() {
  // Update Labels (Numbers)
  lv_label_set_text_fmt(ui_LabelRPM, "%d", currentRPM);
  lv_label_set_text_fmt(ui_LabelSpeed, "%d", currentSpeed);
  lv_label_set_text_fmt(ui_LabelTemp, "%d °C", currentTemp);

  // Update Arcs and Bars (Assumes arcs are configured 0-100 or matching max values in SquareLine)
  // map() functions scale the real values to fit your UI element's max values
  lv_arc_set_value(ui_ArcRPM, currentRPM); 
  lv_arc_set_value(ui_ArcSpeed, currentSpeed); // Assuming arc is set 0-200 in SquareLine
  lv_arc_set_value(ui_ArcTemp, currentTemp); 
  
  // Set Boost Bar (LV_ANIM_ON enables smooth filling if enabled in SquareLine)
  lv_bar_set_value(ui_BarBoost, currentBoostKpa, LV_ANIM_ON);
  lv_label_set_text_fmt(ui_LabelBoost1, "%d Kpa", currentBoostKpa);
}
