#include "rgb_lcd.h"
rgb_lcd lcd;

uint8_t counterLatch = 0; //Latch for Button
uint8_t convertCounter = 0; //Default Button presses

uint8_t modeLatch = 0; //Latch for modeButton
uint8_t modeCounter = 0; //mode Button presses

//==============================================

uint8_t rangeFinder( uint8_t a )
{
  uint16_t rawData = 0;
  uint16_t converted = 0;

  pinMode(3,OUTPUT);

  digitalWrite(3,LOW);
  digitalWrite(3,HIGH);
  delayMicroseconds(5);
  digitalWrite(3,LOW);

  pinMode(3,INPUT); //Set to read
  rawData = pulseIn(3,HIGH);  //Reads the data

  converted = rawData / 60.6;

  return converted;
}

//==============================================
// Results Function

void results ()
{

  //======================
  //CM/INCHES Display Crossroad
  lcd.setCursor(0,0);
  lcd.print("DIS:");
  lcd.setCursor(4,0);

  if (convertCounter % 2 == 0) // Converts cm to inches display
  {
    if(converter(1) < 0.7874)  // 0.7874in = 2 cm
    {
      lcd.setCursor(0,0);
      lcd.print("Too Close !");
    }
    else if(converter(1) > 78.74)  // 78.74in = 200 cm
    {
      lcd.setCursor(0,0);
      lcd.print("Too Far !");
    }
    else
    {
      lcd.print(converter(1));
      lcd.print(" inch");
    }
  }
  else // CM Display
  {
    if(converter(1) < 2)
    {
      lcd.setCursor(0,0);
      lcd.print("Too Close !");
    }
    else if(converter(1) > 150)
    {
      lcd.setCursor(0,0);
      lcd.print("Too Far !");
    }
    else
    {
      lcd.print(converter(1));
      lcd.print("cm");
    }
  }

  //=======================
  // Convert Button Press Display
  lcd.setCursor(0,1);
  lcd.print("BP:");
  lcd.setCursor(3,1);
  lcd.print(convertCounter);

  //=======================
  // Screen wipes
  delay(200);
  lcd.clear();
}

//==============================================
// Conversion Function
uint16_t converter ( uint8_t a )
{
  uint16_t converted;

  if (convertCounter % 2 == 0) // Converts cm to inches display
  {
    converted = (rangeFinder(1) * 0.3937);
  }
  else
  {
    converted = rangeFinder(1);
  }

  return converted;
}

//==============================================
// SETUP ...
void setup() {
  
  lcd.begin(16,2);
  lcd.blink();

  pinMode(45,INPUT); // Convert Button Input
  pinMode(1,INPUT);  // Mode Button Input

}
//==============================================
// LOOP
void loop() 
{
  uint8_t convertPress;
  uint8_t modePress;

  convertPress = digitalRead(45); // Reads the Convert button input
  modePress = digitalRead(1);     // Reads the Mode button input

  //===========================
  // Mode Button Latch
  if (modePress == 1 && modeLatch == 0)
  {
    modeCounter = modeCounter + 1;
    modeLatch = 1;
  }
  else if (modePress == 0)
  {
    modeLatch = 0;
  }

  //===========================
  // Mode CrossRoad

  if (modeCounter % 2 == 0)   // Calibrate Route
  {
    lcd.print("Range:");
    lcd.print(converter(1));

    lcd.setCursor(0,1);
    lcd.print("Factor:");
    delay(200);
    lcd.clear();

  }
  else // Normal Range Finding Route
  {
    if (convertPress == 1 && counterLatch == 0)
    {
    convertCounter = convertCounter + 1;
    counterLatch = 1;
    }
    else if (convertPress == 0)
    {
      counterLatch = 0;
    }

    results();
  }

}