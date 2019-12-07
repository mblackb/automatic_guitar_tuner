#include <LiquidCrystal.h>
#include <Wire.h>
#include <AudioFrequencyMeter.h>

LiquidCrystal lcd(11,10,9,8,7,6);
AudioFrequencyMeter meter;

const int motorLeft = 5;
const int motorRight = 4; 

const double string1Frequency = 82.4;
const double string2Frequency = 110;
const double string3Frequency = 146.8;
const double string4Frequency = 196;
const double string5Frequency = 246.9;
const double string6Frequency = 329.6;
      double sf=0,
      frequency = 0;
const int button = 1;
      int counter = 0;
      int buttonState = 0, buttonLastState =0 ;
      
void stringNumber(){ //Check for selected string and assign accepted frequency
  buttonState = digitalRead(button);
  if(buttonState != buttonLastState){
    if(digitalRead(button)==LOW){
      if (counter <6){
        counter = counter + 1;
      }
      else{
        counter = 1;
      }
    }
  }
  switch (counter){ 
      case 1:
      sf = string1Frequency;
      break;
      case 2:
      sf = string2Frequency;
      break;
      case 3:
      sf = string3Frequency;
      break;
      case 4:
      sf = string4Frequency;
      break;
      case 5:
      sf = string5Frequency;
      break;
      case 6:
      sf = string6Frequency;
      break;
      default:
      sf = 0;
      break;
    }
  delay(50);
  buttonLastState = buttonState;
} //end stringNumber function

int errorCheck(double f){ //check for accepted pass bands, if stop then trigger error
  int e = 0;
  switch (counter){
      case 1:
      if ( f < 40.7 | f > 166){
       e = 1;
      }
      break;
      case 2:
      if ( f < 54.5 | f > 222){
        e = 1;
      }
      break;
      case 3:
      if ( f < 72.9 | f > 295.6){
        e = 1;
      }
      break;
      case 4:
     
      if ( f < 97.5 | f > 394){
        e = 1;
        }
      break;
      case 5:
     
      if ( f < 245 | f> 495){
        e =  1;
      }
      break;
      case 6:
      if ( f < 164 | f > 661 ){
        e = 1;
      }
      break;
      default:
        e = 0;
      break;
  }
  return e;
}

void LCDrange(float freq, float noteFreq){ //calculate the difference between detected and accepted frequencies
    double difference = freq - noteFreq; 
    lcd.print(difference);
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(motorLeft,OUTPUT);
  pinMode(motorRight,OUTPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("please select:");
  delay(3000);
  lcd.clear();
  //frequency detection parameters -> tunes frequency detection
  meter.setBandwidth(10.00, 800); 
  meter.setAmplitudeThreshold(70);
  meter.begin(A0, 45000);             // Intialize A0 at sample rate of 45kHz
}

void loop() {
  // put your main code here, to run repeatedly:
    frequency = meter.getFrequency(); //detect frequency
    stringNumber();
    lcd.setCursor(0, 0);
    lcd.print("STR:"); 
    lcd.setCursor(4, 0);lcd.print(counter);
    lcd.setCursor(8, 0);lcd.print(sf);
    lcd.setCursor(0, 1);
    if(errorCheck(frequency) == 1){
       lcd.print("Error");
    }
    if(errorCheck(frequency) == 0) {
        lcd.print(frequency); 
        lcd.setCursor(8, 1); LCDrange(frequency,sf);
        delay(500);
        if(frequency > 0 & counter > 0){ 
             if (frequency > (sf + 1)) {
                digitalWrite(motorLeft, HIGH);
                delay(100);
                digitalWrite(motorLeft, LOW);
             }
             else if (frequency < (sf - 1)){
                digitalWrite(motorRight,HIGH);
                delay(100);
                digitalWrite(motorRight,LOW);
             }
        }
    }
}
