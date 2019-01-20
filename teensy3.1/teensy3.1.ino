#include <Key.h>
#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS]=
{
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};
char keysd[ROWS][COLS] = {
  {'1','2','3'}, /*its fucking flipped oh */ 
  {'4','5','6'},                            
  {'7','8','9'},                             
  {'#','0','*'}                               
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 12, 11, 10 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 13

/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun's flex sensors
  (https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it's being bent), the
voltage at A0 should decrease.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FLEX_PIN = A1; // Pin connected to voltage divider output
const int FLEX_PIN2 = A3; //Pin for next voltage divider output 


const int NUMATTEMPTS=1; //change this to see how mnay attempts u have to try a code. 

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg

#include <Wire.h>
#include <LPS.h>


//this all needs to be in setup actually tbh
  int keycode[4]={ 0,0,0,0};
int keycheck[4]={-1,-1,-1,-1};
int countcheck=0;
int startflag=0; //without star, we can't start 


LPS ps;

void setup() 
{

  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  Wire.begin();


  
   if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }

  ps.enableDefault();
}

void loop() 
{
  // Read the ADC, and calculate voltage and resistance from it
  int flexADC1 = analogRead(FLEX_PIN);
  int flexADC2 = analogRead(FLEX_PIN2);
  float flexV1 = flexADC1 * VCC / 1023.0;
  float flexR1 = R_DIV * (VCC / flexV1 - 1.0);
    float flexV2 = flexADC2 * VCC / 1023.0;
  float flexR2 = R_DIV * (VCC / flexV2 - 1.0);
  Serial.println("Resistance right one: " + String(flexR1) + " ohms");
    Serial.println();
  Serial.println("Resistance left one : " + String(flexR2) + " ohms");
    Serial.println();
  

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle1 = map(flexR1, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
  float angle2 = map(flexR2, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
  Serial.println("Bend right one: " + String(angle1) + " degrees");
  Serial.println();
    Serial.println("Bend left one: " + String(angle2) + " degrees");
  Serial.println();


  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  Serial.print("p: ");
  Serial.print(pressure);
  Serial.print(" mbar\ta: ");
  Serial.print(altitude);
  Serial.print(" m\tt: ");
  Serial.print(temperature);
  Serial.println(" deg C");

//changinf starter code from this link
//http://playground.arduino.cc//Main/KeypadTutorial


//adding functionality, if too many attempts of failing, you will get locked out.

  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
      {
        //digitalWrite(ledpin, LOW);
        //break;
        //start appending
        if(!startflag)
        {
          startflag=1;
          Serial.println("start entering keys!");
        }
        else
        {
           Serial.println("you already hit start entering keys.");
           startflag=1;
        }
        break;
      }
      case '#':
      {
        if(startflag)
        {
          //check if they even have 4 digs
          if(countcheck<4)
          {
            Serial.println("you don't have 4 keys yet, come back and try again next time");
          }
          else
           {
           startflag=0;
           bool correctcode=1;
           for(int c=0;c<4;c++)
           {
              if(keycheck[c]==keycode[c])
              {
                continue;
              }
              else
              {
                Serial.println("lol code wrong");
                //break;
               // Serial.println("keycheck[c]");
               // Serial.println(keycheck[c]);

               // Serial.println("keycode[c]");
               // Serial.println(keycode[c]);
               correctcode=0;
               //and also, reset
               countcheck=0;
               startflag=0;
              }
           }
           if(correctcode)
           {
           Serial.println("nice code correct, gj");
           countcheck=0;
            //reset keycheck back to nothing
              for(int c=0;c<4;c++)
              {
                keycheck[c]=-1;
              }
              startflag=0;
            }
           }
        }
        else
        {
          Serial.println("need to hit the star button to start");
        }
        break;
    }
        
        //digitalWrite(ledpin, HIGH);
        //break; //where the fuck is the rest of my keys and why are most of them routed wrong 
      default:
      {
        if(startflag)
        {
          //check size before dealing with this
          if(countcheck<4)
          {
          Serial.println(key);
        //append key to array
          keycheck[countcheck]=key-'0'; //char - 0 char to map to int
          countcheck++;
          }
          else
          {
            Serial.println("stop you have too many keys submit it already");
          }
        }
      
        
        else
        {
          Serial.println("you need to hit star button to start entering your keys");
        }
       }
    }
  }
//star is start, pound is end. want to receive a code, and then output to serial monitor if code is correct . default "correct" code is 0000
  delay(500);
}
