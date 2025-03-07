#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <magyar_font.h>
#include <SPI.h>

#define TFT_CS 17
#define TFT_RST 21
#define TFT_DC 22
#define TFT_DISPLAY_LED 28

// Kijelző inicializálása
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char* magyar_szoveg = "árvíztűrő tükörfúrógép ÁRVÍZTŰRŐ TÜKÖRFÚRÓGÉP és további magyar szöveg megjelenítése egy teljes képernyőn át valamint és további magyar szöveg megjelenítése egy teljes képernyőn át valamint";

int cursorX = 10;
int cursorY = 22;
int n = 1;
int8_t sa;
int8_t xa;  // glyph xAdvance érték
uint8_t char_width = 20;
uint16_t x_offset = 0, y_offset = 30;
const int lineHeight = 32; // Egy karakter magasága
char rd = 0;
uint16_t c_gfx, c1o, c2o;

/// @brief 2 byte kódolású UTF-8 magyar karakterek megjelenítése 
/// @param const char* text 
void drawUTF8Text(const char* text) {
    while (*text)
        {
        c_gfx = *text++;
        if (c_gfx < 195) xa = tft.drawChar(0 + x_offset, y_offset, c_gfx, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //egy byte-os karakter ASCII
        else if (c_gfx == 195) {
            c_gfx = *text++;
            xa = tft.drawChar(0 + x_offset, y_offset, c_gfx + 64, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //egy byte-os karakter  áÁúíÍüÜöÖúÚóÓéÉ
            }
        else if (c_gfx == 197) {
            c_gfx = *text++;
            if (c_gfx == 144) xa = tft.drawChar(x_offset, y_offset, c_gfx + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1);       // Ő?
            else if (c_gfx == 176) xa = tft.drawChar(x_offset, y_offset, (unsigned char)368, ILI9341_WHITE, ILI9341_BLACK, 1, 1);          // Ű
            else if (c_gfx == 177) xa = tft.drawChar(x_offset, y_offset, c_gfx + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1);  // ű
            else if (c_gfx == 145) xa = tft.drawChar(x_offset, y_offset, c_gfx + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1);  // ő
            }
        x_offset += xa;               // proporcionális betűk távolsága
        if (x_offset >= 310) {        //soremelés
            x_offset = 0;
            y_offset += lineHeight;
            }
        }
    }

void setup() {
    #ifdef DEBUG
    Serial1.begin(115200);
    #else
    Serial.begin(115200);
    Serial.print("No debug+");
    #endif
    pinMode(TFT_DISPLAY_LED, OUTPUT); // ILI9341 kijelző LED bekapcsolása
    gpio_set_dir(TFT_DISPLAY_LED, GPIO_OUT);
    gpio_put(TFT_DISPLAY_LED, 1);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(&calibri16pt8b); // Egyedi font használata
    tft.setCursor(0, 20);
    drawUTF8Text(magyar_szoveg);
    Serial1.println("Írj be karaktereket a soros monitoron!");
    }

void loop() {                     
    if((sa = Serial1.available())!=0){
    tft.fillScreen(ILI9341_BLACK);
    switch (sa) {
        case 1: Serial1.read();       //UTF8 2byte characrer to GFX char decoder
        break;
        case 2: {                    // 1 byte+ enter
        Serial1.print("sa: ");
        Serial1.println(sa);
        tft.fillRect(0, 0, 320, 31, ILI9341_BLACK);   // első sor törlése
        uint8_t c1 = Serial1.read();
        uint8_t c2 = Serial1.read();
        Serial1.print('\t');
        Serial1.print(c2);
        Serial1.print('\t');
        Serial1.println(c1);
        tft.drawChar(100, 30, (c1), ILI9341_WHITE, ILI9341_BLACK, 1, 1);
        }
        break;
        case 3: {
        Serial1.print("sa: ");
        Serial1.println(sa);
        tft.fillRect(0, 0, 200, 31, ILI9341_BLACK);
        uint8_t c1 = Serial1.read();
        uint8_t c2 = Serial1.read();
        uint8_t c3 = Serial1.read();
        tft.setCursor(0,30);
        tft.print(c3);
        tft.setCursor(60,30);
        tft.print(c2);
        tft.setCursor(120,30);
        tft.println(c1);

        if (c1 == 195) tft.drawChar(200, 30, (c2 + 64), ILI9341_RED, ILI9341_WHITE, 1, 1); //ASCII
        else if (c1 == 197 && c2 == 144) tft.drawChar(200, 30, c2 + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //hol van a Ő?
        else if (c1 == 197 && c2 == 176) tft.drawChar(200, 30, (unsigned char)368, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //hol van a Ű?
        else if (c1 == 197 && c2 == 177) tft.drawChar(200, 30, c2 + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //hol van a ű?
        else if (c1 == 197 && c2 == 145) tft.drawChar(200, 30, c2 + 192, ILI9341_WHITE, ILI9341_BLACK, 1, 1); //hol van a őüŐÚÖÜ?
        }
        break;
        }
    delay(100);
    sa=0;        // sa törlésével várakozó hurokba kerül
    }
}
