#include <iarduino_IR_RX.h>

#include <GyverOS.h>
#include <EEPROM.h>

#include <color_utility.h>
#include <FastLEDsupport.h>
#include <microLED.h>

#include "FastLED.h"          // библиотека для работы с лентой

#define LED_COUNT 36          // число светодиодов в кольце/ленте
#define LED_DT 6             // пин, куда подключен DIN ленты

microLED<LED_COUNT, LED_DT, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;
#include <FastLEDsupport.h>    // нужна для шума

iarduino_IR_RX IR(2);                            // Объявляем объект IR, с указанием вывода к которому подключён ИК-приёмник


int max_bright = 100;          // максимальная яркость (0 - 255)
boolean adapt_light = 0;       // адаптивная подсветка (1 - включить, 0 - выключить)

int count = 0;
byte fav_modes[] = {47, 48};  // список "любимых" режимов
byte num_modes = sizeof(fav_modes);         // получить количество "любимых" режимов (они все по 1 байту..)
unsigned long change_time, last_change, last_bright;
int new_bright;


// цвета мячиков для режима
byte ballColors[3][3] = {
  {0xff, 0, 0},
  {0xff, 0xff, 0xff},
  {0   , 0   , 0xff}
};

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
int ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 100;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int currentHue = EEPROM.read(1);
int currentBrigthSpeed = EEPROM.read(2);
volatile byte currentMode = EEPROM.read(3);

int tempHue;
int tempBrightSpeed;

int thisindex = 0;
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR

volatile uint32_t btnTimer;
volatile byte modeCounter;
volatile boolean changeFlag;

volatile byte ledMode = currentMode;

#define BUTT_UP     16718055
#define BUTT_DOWN   16730805
#define BUTT_LEFT   16716015
#define BUTT_RIGHT  16734885
#define BUTT_OK     16726215
#define BUTT_1      16753245
#define BUTT_2      16736925
#define BUTT_3      16769565
#define BUTT_4      16720605
#define BUTT_5      16712445
#define BUTT_6      16761405
#define BUTT_7      16769055
#define BUTT_8      16754775
#define BUTT_9      16748655
#define BUTT_0      16750695
#define BUTT_STAR   16738455
#define BUTT_HASH   16756815

GyverOS<2> OS;
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

void updateHue() {
  currentHue = constrain(tempHue, 0, 255);
  EEPROM.update(1, currentHue);
  change_mode(currentMode);
}
void updateBrigthSpeed() {
  currentBrigthSpeed = constrain(tempBrightSpeed, 0, 20);
  EEPROM.update(2, currentBrigthSpeed);
  change_mode(currentMode);
}

void setup()
{
  Serial.begin(9600);

  OS.attach(0, updateHue, 500);
  OS.stop(0);
  OS.attach(1, updateBrigthSpeed, 500);
  OS.stop(1);


  currentHue = EEPROM.read(1);
  currentBrigthSpeed = EEPROM.read(2);
  currentMode = EEPROM.read(3);

  Serial.begin(9600);              // открыть порт для связи
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость

  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
}

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void loop() {
  IR.begin();
  //Serial.println("currentMode " + (String)currentMode);
  change_mode(currentMode);

  if (IR.check()) {

    switch (IR.data) {
      // режимы
      case BUTT_1:
        currentMode = 2;
        EEPROM.update(3, currentMode);
        change_mode(2);
        break;
      case BUTT_2:
        currentMode = 3;
        EEPROM.update(3, currentMode);
        change_mode(3);
        break;
      case BUTT_3:
        currentMode = 4;
        EEPROM.update(3, currentMode);
        change_mode(4);
        break;
      case BUTT_4:
        currentMode = 5;
        EEPROM.update(3, currentMode);
        change_mode(5);
        break;
      case BUTT_5:
        currentMode = 6;
        EEPROM.update(3, currentMode);
        change_mode(6);
        break;
      case BUTT_6:
        currentMode = 7;
        EEPROM.update(3, currentMode);
        change_mode(7);
        break;
      case BUTT_7:
        currentMode = 8;
        EEPROM.update(3, currentMode);
        change_mode(8);
        break;
      case BUTT_8:
        currentMode = 9;
        EEPROM.update(3, currentMode);
        change_mode(9);
        break;
      case BUTT_9:
        currentMode = 10;
        EEPROM.update(3, currentMode);
        change_mode(10);
        break;
      case BUTT_0:
        currentMode = 0;
        EEPROM.update(3, currentMode);
        change_mode(0);
        break;
      case BUTT_STAR: break;
        break;
      case BUTT_HASH: break;
        break;
      case BUTT_OK: break;
        break;
      case BUTT_UP: if (currentMode == 10) {
          tempHue = currentHue + 10;
          OS.exec(0);
        }
        break;
      case BUTT_DOWN: if (currentMode == 10) {
          tempHue = currentHue - 10;
          OS.exec(0);
        }
        break;
      case BUTT_LEFT: if (currentMode == 10) {
          tempBrightSpeed = currentBrigthSpeed - 1;
          OS.exec(1);
        }
        break;
      case BUTT_RIGHT: if (currentMode == 10) {
          tempBrightSpeed = currentBrigthSpeed + 1;
          OS.exec(1);
        }
        break;
      default: false;   // если не распознали кнопку, не обновляем настройки!
        break;
    }

    /*Serial.println("currentMode " + (String)currentMode);
    Serial.println("currentBrigthSpeed " + (String)currentBrigthSpeed);
    Serial.println("tempBrightSpeed " + (String)tempBrightSpeed);
    Serial.println("currentHue " + ( String)currentHue);
    Serial.println("tempHue " + ( String)tempHue);
    Serial.println("ledMode " + (String)ledMode);*/

  }

  switch (ledMode) {
    case 999: break;                           // пазуа
    case  2:   fireEffect(255); break;
    case  3:   fireEffectBlue(255); break;
    case  4:   one_color_all(255, 0, 0); LEDS.show(); break;
    case  5:   one_color_all(0, 255, 0); LEDS.show(); break;
    case  6:   one_color_all(0, 0, 255); LEDS.show(); break;
    case  7:   one_color_all(255, 255, 0); LEDS.show(); break;
    case  8:   one_color_all(0, 255, 255); LEDS.show(); break;
    case  9:   one_color_all(255, 0, 255); LEDS.show(); break;
    case  10:  pulse_one_color_all(currentBrigthSpeed, currentHue); break;

  }

}


void btnISR() {
  if (millis() - btnTimer > 150) {
    btnTimer = millis();  // защита от дребезга
    if (++modeCounter >= num_modes) modeCounter = 0;
    ledMode = fav_modes[modeCounter];    // получаем новый номер следующего режима
    change_mode(ledMode);               // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
    changeFlag = true;
  }
}

void change_mode(int newmode) {
  thissat = 255;
  switch (newmode) {
    case 0: one_color_all(0, 0, 0); LEDS.show(); break; //---ALL OFF
    case 1: one_color_all(255, 255, 255); LEDS.show(); break; //---ALL ON
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
  }
  //bouncedirection = 0;
  one_color_all(0, 0, 0);
  ledMode = newmode;
}
