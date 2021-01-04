/**************************************************************************
  
 **************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <dhtnew.h>

#define TFT_CS   4  // CS
#define TFT_DC   7  // A0
#define TFT_MOSI 6  // Data out SDA
#define TFT_SCLK 5  // Clock out SCK
#define TFT_RST  8  // RST

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
DHTNEW dhtSensor(10);

float p = 3.1415926;

float temp;
float humd;

float temp_prev = 0;
float humd_prev = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("LY2EN air module testing"));

  tft.initR(INITR_BLACKTAB);      

  // tft.initR(INITR_GREENTAB);      
  // tft.setSPISpeed(40000000);

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  time = millis() - time;

  Serial.println(time, DEC);

  Serial.println("done");

}

void loop() {

// if (millis() - mySensor.lastRead() > 2000)
  dhtSensor.read();
  
  temp = dhtSensor.getTemperature();
  humd = dhtSensor.getHumidity();

  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_YELLOW);
  tft.println("*LY2EN*");

  tft.setTextSize(2);

  tft.setTextColor(ST77XX_WHITE);
  tft.print("Temp:");
  tft.setTextColor(ST77XX_RED);
  tft.print(stateChange(temp, temp_prev));
  tft.println(temp,1);
  
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Hum: ");
  tft.setTextColor(ST77XX_BLUE);
  tft.print(stateChange(humd, humd_prev));
  tft.println(humd,1);


  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Uptime: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.println(" sec.");

  temp_prev = temp;
  humd_prev = humd;
  
  delay(10000);
}

char stateChange(float curr, float prev) {
  if (curr > prev) return '+';
  if (curr < prev) return '-';
  return ' ';  
}
