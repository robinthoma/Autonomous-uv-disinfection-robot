#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];


UTFT myGLCD(ILI9481,38,39,40,41,42);// ElecFreaks TFT2.2SP Shield



// constants won't change. They're used here to set pin numbers:
const int Reedswitch = 11;     // the number of the pushbutton pin
const int ledPin =  10;
const int relay = 9; 
const int statusled = 12; 

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
   randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  digitalWrite(statusled, HIGH);
  // initialize the LED pin as an output:
  
  pinMode(ledPin, OUTPUT);
  pinMode(relay, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(Reedswitch, INPUT);
}

void loop() {


// Clear the screen and draw the frame
  //myGLCD.clrScr();

  // read the state of the pushbutton value:
  buttonState = digitalRead(Reedswitch);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    digitalWrite(relay, HIGH);
   myGLCD.clrScr();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("UVBOT is Charging!", CENTER, 93);
  
  } else {
    
    digitalWrite(ledPin, LOW);
    digitalWrite(relay, LOW);
     myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Charging circuit is ON!", CENTER, 93);
  delay(20);
  }
}
