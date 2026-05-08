English:
My project configured for older vehicles using K-Line and the KWP2000 protocol, uses an Arduino ESP32
together with a TFT 2.8" 240x320px display and relies on a WiFi diagnostic adapter with an ELM327 chip, specifically:
Xtrons Auto Diagnostics OBD II with Wi-Fi OBD04 (https://www.evtech.sk/xtrons-autodiagnostika-obd-ii-s-wi-fi-obd04/)
Required libraries:
Wifi.h
TFT_eSPI.h
lvgl.h
You must modify your TFT_eSPI library configuration file, User_Setup.h, according to your TFT display wiring to the ESP32
and according to your specific display and the driver it uses, my configuration:
define ST7789_DRIVER      # display driver
define TFT_INVERSION_OFF  # without this the colors on the display were inverted (black>white)
define TFT_WIDTH 240      # display width
define TFT_HEIGHT 320     # display height
TFT_MISO 21   # SDO(MISO) -> D21
TFT_MOSI  5   # SDI(MOSI) -> D5
TFT_SCLK 18   # SCK       -> D18
TFT_CS   15   # CS        -> D15
TFT_DC    4   # DC        -> D4
TFT_RST   2   # RESET     -> D2
Next, drag the pre-configured and designed GUI (files ui_... .c/.h) into a single folder and open them together
with Custom_OBD_Dash.ino in the Arduino IDE and set the ESP32 Dev Module in the Board Manager, verify and upload... the device should run smoothly :)


Slovak:
Moj projekt nakonfigurovany pre stare vozidla pouzivajecu K-Line a protokol KWP2000, pouziva Arduino ESP32 
spolocne s TFT 2.8" 240x320px displejom sa spolieha na WiFi diagnosticky adapter s cipom ELM327 presnejsie:
Xtrons Autodiagnostika OBD II s Wi-Fi OBD04 (https://www.evtech.sk/xtrons-autodiagnostika-obd-ii-s-wi-fi-obd04/)

Nutne kniznice:
Wifi.h
TFT_eSPI.h
lvgl.h

Musite si upravit svoj konfiguracny subor kniznice TFT_eSPI, User_Setup.h, podla vaseho zapojenia TFT displeja ku ESP32
a podla vaseho specifickeho displeja a ovladaca ktory pouziva, moja konfiguracia:

define ST7789_DRIVER      # ovladac displeja
define TFT_INVERSION_OFF  # bez tohto boli farby na displeji obratene (cierna>biela)
define TFT_WIDTH 240      # sirka displeja
define TFT_HEIGHT 320     # vyska displeja

TFT_MISO 21   # SDO(MISO) -> D21
TFT_MOSI  5   # SDI(MOSI) -> D5
TFT_SCLK 18   # SCK       -> D18
TFT_CS   15   # CS        -> D15
TFT_DC    4   # DC        -> D4
TFT_RST   2   # RESET     -> D2

Dalej pretiahnite do jedneho priecinku predom nakonfigurovane a navrhnute GUI (subory ui_... .c/.h) a otvorte ich spolocne
s Custom_OBD_Dash.ino v Arduino IDE a nastavte si v Board Managery ESP32 Dev Module, verify a upload... zariadenie by malo ficat :)
