/**************************************************************************
  
 **************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <dhtnew.h>
#include <MQUnifiedsensor.h>

#define TFT_CS   4  // CS
#define TFT_DC   7  // A0
#define TFT_MOSI 6  // Data out SDA
#define TFT_SCLK 5  // Clock out SCK
#define TFT_RST  8  // RST

#define ST7735_GRAY    0x8410

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
DHTNEW dhtSensor(10);

#define board "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10
#define RatioMQ135CleanAir 3.6

MQUnifiedsensor MQ135Sensor(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

float temp;
float humd;

float temp_prev = 0;
float humd_prev = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("LY2EN air module testing"));

  MQ135Sensor.setRegressionMethod(1);
  MQ135Sensor.init();

  tft.initR(INITR_BLACKTAB);
  // tft.initR(INITR_GREENTAB);      
  // tft.setSPISpeed(40000000);

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  time = millis() - time;

  Serial.println(time, DEC);

  Serial.println("done");

}
/*
void loop() {

  // calibration 
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135Sensor.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135Sensor.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135Sensor.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); 
    while(1);
    }
  if(calcR0 == 0) {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); 
    while(1);
    }
  // end calibration

}

*/

void loop() {

// if (millis() - mySensor.lastRead() > 2000)
  dhtSensor.read();
  
  temp = dhtSensor.getTemperature();
  humd = dhtSensor.getHumidity();

  MQ135Sensor.update();

  MQ135Sensor.setA(605.18); MQ135Sensor.setB(-3.937);
  float CO = MQ135Sensor.readSensor();

  MQ135Sensor.setA(77.255); MQ135Sensor.setB(-3.18);
  float Alcohol = MQ135Sensor.readSensor();

  MQ135Sensor.setA(110.47); MQ135Sensor.setB(-2.862);
  float CO2 = MQ135Sensor.readSensor();

  MQ135Sensor.setA(44.947); MQ135Sensor.setB(-3.445);
  float Toluene = MQ135Sensor.readSensor();

  MQ135Sensor.setA(102.2 ); MQ135Sensor.setB(-2.473);
  float NH4 = MQ135Sensor.readSensor();

  MQ135Sensor.setA(34.668); MQ135Sensor.setB(-3.369);
  float Acetone = MQ135Sensor.readSensor();


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

  tft.setTextColor(ST7735_GRAY);
  tft.print("CO:  ");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(CO,1);

  tft.setTextColor(ST7735_GRAY);
  tft.print("CO2: ");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(CO,1);

  tft.setTextColor(ST7735_GRAY);
  tft.print("NH4: ");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(NH4,1);

  tft.setTextColor(ST7735_GRAY);
  tft.print("Alco:");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(Alcohol,1);

  tft.setTextColor(ST7735_GRAY);
  tft.print("Acet:");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(Acetone,1);

  tft.setTextColor(ST7735_GRAY);
  tft.print("Tolu:");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(Toluene,1);

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
