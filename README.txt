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
