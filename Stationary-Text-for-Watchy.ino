#include <Watchy.h> //include the Watchy library
#include "NunitoSans_Bold17pt7b.h"
#include "NunitoSans_Light17pt7b.h"
#include "icons.h" //stock icons
#include "NunitoSans_ExtraLight10pt7b.h" //for step counter, date and temperature
        
#define DARKMODE true
        

class WatchFace : public Watchy { //inherit and extend Watchy class
  public:

        void drawSteps();
        void drawWeather();
        void drawBattery();
        void drawDate();
        
    void drawWatchFace() { //override this method to customize how the watch face looks
      uint16_t lines = 0;
      const char *lows [10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
      const char *teensone [11] = {"","ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
      const char *teenstwo [11] = {"", "","", "teen", "teen", "teen", "teen", "teen", "teen", "teen", "teen"};
      const char *tens [10] = {"zero", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
     

      //drawbg
      display.fillScreen(GxEPD_BLACK);
      display.setTextColor(GxEPD_WHITE);
      display.setTextWrap(false);

       //draw Steps
      uint32_t stepCount = sensor.getCounter();                
      display.setFont(&NunitoSans_ExtraLight10pt7b);    
      display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.setCursor(35, 190); 
      display.println(stepCount);

      //draw Date
      
          String monthStr = monthShortStr(currentTime.Month);
          String dayOfWeek = dayShortStr(currentTime.Wday);
          display.setFont(&NunitoSans_ExtraLight10pt7b);
          char* dateStr;
          display.setCursor(15, 20);
          display.println(dayOfWeek);        

      //draw Battery
      const uint8_t BATTERY_SEGMENT_WIDTH = 7;
      const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
      const uint8_t BATTERY_SEGMENT_SPACING = 9;
      
      display.drawBitmap(154, 10, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);    
      display.fillRect(159, 15, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments    
      int8_t batteryLevel = 0;    
      float VBAT = getBatteryVoltage();    
      if(VBAT > 4.1){
        batteryLevel = 3;    
        }    
        else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
        }    
        else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;    
        }        
        else if(VBAT <= 3.80){
        batteryLevel = 0;    
        }    
        
        for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 15, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);    
        }


      //draw Weather
      const uint8_t WEATHER_ICON_WIDTH = 48;
      const uint8_t WEATHER_ICON_HEIGHT = 32;      
      
      weatherData currentWeather = getWeatherData();    
      int8_t temperature = currentWeather.temperature;    
      int16_t weatherConditionCode = currentWeather.weatherConditionCode;       
      
      display.setFont(&NunitoSans_ExtraLight10pt7b);    
      int16_t  x1, y1;    
      uint16_t w, h;    
      display.getTextBounds(String(temperature), 115, 190, &x1, &y1, &w, &h);    
      display.setCursor(115 - w, 190);    
      display.println(temperature);    
      display.drawBitmap(120, 170, TEMP_UNIT == "metric" ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);    
      const unsigned char* weatherIcon;
      
      //https://openweathermap.org/weather-conditions    
      if(weatherConditionCode > 801){//Cloudy    
        weatherIcon = cloudy;    
        }else if(weatherConditionCode == 801){//Few Clouds    
          weatherIcon = cloudsun;      
          }else if(weatherConditionCode == 800){//Clear    
            weatherIcon = sunny;      
            }else if(weatherConditionCode >=700){//Atmosphere    
              weatherIcon = cloudy;     
              }else if(weatherConditionCode >=600){//Snow    
                weatherIcon = snow;    
                }else if(weatherConditionCode >=500){//Rain    
                  weatherIcon = rain;  
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = rain;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = rain; 
    }
    display.drawBitmap(155, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);


      //drawtime
    lines += 1;
      display.setCursor(8, lines*47-5);
      display.setFont(&NunitoSans_Bold17pt7b);
      if (currentTime.Hour == 0) {
        display.print(tens[2]);
        lines += 1;
        display.setCursor(8, lines*47-5);
        display.print(lows[4]);
      } else if (currentTime.Hour < 10) {
        display.print(lows[currentTime.Hour]);
      } else if (currentTime.Hour < 20) {
        display.print(teensone[currentTime.Hour-9]);
        if (currentTime.Hour > 12) {
          lines += 1;
          display.setCursor(8, lines*47-5);
          display.print(teenstwo[currentTime.Hour%10]);
        }
      } else {
        display.print(tens[currentTime.Hour/10]);
        if (currentTime.Hour%10 > 0) {
          lines += 1;
          display.setCursor(8, lines*47-5);
          display.print(lows[currentTime.Hour%10]);
        }
      }

      lines += 1;
      display.setCursor(8, lines*47-5);
      display.setFont(&NunitoSans_Light17pt7b);
      if (currentTime.Minute == 0) {
        display.print("o'clock");
      } else if (currentTime.Minute < 10) {
          display.print(" ");
          lines += 1;
          display.setCursor(8, lines*47-5);
          display.print(lows[currentTime.Minute]);
      } else if (currentTime.Minute < 20) {
        display.print(teensone[currentTime.Minute]);
       
      } else {
        display.println(tens[currentTime.Minute/10]);
        if (currentTime.Minute%10 > 0) {
          lines += 1;
          display.setCursor(8, lines*47-5);
          display.print(lows[currentTime.Minute%10]);
        }
      }                
    }
};

WatchFace m; //instantiate your watchface

void setup() {
  m.init(); //call init in setup
}

void loop() {
  // this should never run, Watchy deep sleeps after init();
}
