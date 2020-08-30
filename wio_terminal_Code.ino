#include <SPI.h>

#define TFT_GREY 0x5AEB
#include"LIS3DHTR.h"
LIS3DHTR<TwoWire> lis;
#include"TFT_eSPI.h"
#include"Free_Fonts.h" //include the header file
TFT_eSPI tft;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

void setup(void) {
  pinMode(WIO_BUZZER, OUTPUT);
    pinMode(A8, INPUT);
    pinMode(D4,OUTPUT);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
   tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    targetTime = millis() + 1000;
  Serial1.begin(115200);
  RTL8720D.begin(115200);
  lis.begin(Wire1);
 
  if (!lis) {
    Serial.println("ERROR");
    while(1);
  }
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); //Scale range set to 2g
}
 
void loop() {
  float x_values, y_values, z_values;
  x_values = lis.getAccelerationX();
  y_values = lis.getAccelerationY();
  z_values = lis.getAccelerationZ();
   int uvsensor = analogRead(A8);
  
 if (RTL8720D.available())
            {

                int inbyte = RTL8720D.read();

                if (inbyte>4){
                
                digitalWrite(D4,HIGH);
                Serial.print("inbyte:"); Serial.write(inbyte);
                tft.drawString("   Caution! UV lamp is On      ",30,180 );
                }
                else if(inbyte<4)
                {
                  digitalWrite(D4,LOW);
              Serial.print("inbyte:");Serial.print(inbyte);
              tft.drawString("         UV lamp is Off                        ",30,180 ); 
              }
                else {
                }
                }
                      
                
            
   
 
 // Serial.print("X: "); Serial.print(x_values);
 // Serial.print(" Y: "); Serial.print(y_values);
 // Serial.print(" Z: "); Serial.print(z_values);
  //Serial.println();
  if (x_values<0.5)
  {
    
     Serial.print("The Bot is down...stopping systems! ");
     Serial.print("temp:"); Serial.println(lis.getTemperature());
     RTL8720D.write("1");
     analogWrite(WIO_BUZZER, 128);
 
      tft.setFreeFont(FS12);
     tft.drawString("The Bot is down",70,140 );
     delay(50);
     Serial.print("uv value"); Serial.println(uvsensor);
     if (uvsensor> 400){
    Serial.print("UV Lamp is working: "); 
    tft.drawString("   Caution! UV lamp is On      ",30,180 );
   }
   else{
    tft.drawString("         UV lamp is Off                        ",30,180 ); 
   }
    
  }
  else
  {Serial.print("temp:"); Serial.println(lis.getTemperature());
   tft.drawString("     UV Robot      ",70,140 ); 
   RTL8720D.write("0");
   analogWrite(WIO_BUZZER, 0);
   Serial.print("uv value"); Serial.println(uvsensor);
   if (uvsensor> 400){
    Serial.print("UV Lamp is working: ");
    tft.drawString("   Caution! UV lamp is On     ",30,180 );
   }
   else{
    tft.drawString("         UV lamp is Off                        ",30,180 ); 
   }
  }
  delay(50);
   if (targetTime < millis()) {
        // Set next update for 1 second later
        targetTime = millis() + 1000; 

        // Adjust the time values by adding 1 second
        ss++;              // Advance second
        if (ss == 60) {    // Check for roll-over
            ss = 0;          // Reset seconds to zero
            omm = mm;        // Save last minute time for display update
            mm++;            // Advance minute
            if (mm > 59) {   // Check for roll-over
                mm = 0;
                hh++;          // Advance hour
                if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
                    hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
                }
            }
        }


        // Update digital time
        int xpos = 0;
        int ypos = 20; // Top left corner ot clock text, about half way down
        int ysecs = ypos + 24;

        if (omm != mm) { // Redraw hours and minutes time every minute
            omm = mm;
            // Draw hours and minutes
            if (hh < 10) {
                xpos += tft.drawChar('0', xpos, ypos, 8);    // Add hours leading zero for 24 hr clock
            }
            xpos += tft.drawNumber(hh, xpos, ypos, 8);             // Draw hours
            xcolon = xpos; // Save colon coord for later to flash on/off later
            xpos += tft.drawChar(':', xpos, ypos - 8, 8);
            if (mm < 10) {
                xpos += tft.drawChar('0', xpos, ypos, 8);    // Add minutes leading zero
            }
            xpos += tft.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
            xsecs = xpos; // Sae seconds 'x' position for later display updates
        }
        if (oss != ss) { // Redraw seconds time every second
            oss = ss;
            xpos = xsecs;

            if (ss % 2) { // Flash the colons on/off
                tft.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
                tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
                xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
                tft.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            } else {
                tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
                xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
            }

            //Draw seconds
            if (ss < 10) {
                xpos += tft.drawChar('0', xpos, ysecs, 6);    // Add leading zero
            }
            tft.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
        }
    }
}
static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9') {
        v = *p - '0';
    }
    return 10 * v + *++p - '0';
}
