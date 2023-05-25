//Serial Communication lib and variables
#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
SoftwareSerial espSerial(5, 6);
const int rs=4, en=3, d4=8, d5=9, d6=10, d7=11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
String data;

// voltmeter variable declaration
int value1 = 0;
float voltage1;
int value2 = 0;
float voltage2;
int value3 = 0;
float voltage3;
int value4 = 0;
float voltage4;
float R1 = 39000.0;
float R2 = 8000.0;

// Thermister Variables

//These values are in the datasheet
#define RT0 10000   // Ω
#define B 3977      // K
//--------------------------------------
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
//Variables
float RT, VR, ln, TX, T0, VRT;

// ACS712 
const int currentPin = A4;
int sensitivity = 66;
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;


void setup(){
//Serial Communication
espSerial.begin(115200);
//Serial.begin(9600);
pinMode(13,OUTPUT);
 T0 = 25 + 273.15;//Temperature T0 from datasheet, conversion from Celsius to kelvin
lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("Cloud Based BMS");
lcd.setCursor(0,1);
lcd.print(" Data Analytics");
delay(1000);
lcd.clear();
}


void loop()
{
 // voltmeter readings start
 value1 = analogRead(A0);
  float temp = (value1 * 5.0) / 1024.0; 
  voltage1 = temp / (R2/(R1+R2));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("V1-");
  lcd.setCursor(3,0);
  lcd.print(voltage1);
 // Serial.println("VOLT_1");
 // Serial.println(voltage1);
  

   value2 = analogRead(A1);
  float temp2 = (value2 * 5.0) / 1024.0; 
  voltage2 = temp2 / (R2/(R1+R2));
  lcd.setCursor(9,0);
  lcd.print("V2-");
  lcd.setCursor(12,0);
  lcd.print(voltage2);
  //Serial.println("VOLT_2");
  //Serial.println(voltage2);
  


 value3 = analogRead(A2);
  float temp3 = (value3 * 5.0) / 1024.0; 
  voltage3 = temp3 / (R2/(R1+R2));
  lcd.setCursor(0,1);
  lcd.print("V3-");
  lcd.setCursor(3,1);
  lcd.print(voltage3);
  //Serial.println("VOLT_3");
  //Serial.println(voltage3);
  delay(2000);

  // voltmeter readings end

  //Temperature Reading start
VRT = analogRead(A3);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;                 //Conversion to Celsius
  TX+=7;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEMP-");
  lcd.setCursor(5,0);
  lcd.print(TX);
  lcd.setCursor(11,0);
  lcd.print("C");
  //Serial.print("TEMP-");
  //Serial.println(TX);
  
//  //First HIGH TEMP Warning 
  if(45<TX && TX<55){
      high1();
  }
    //Second HIGH TEMP Warning 
    else if(TX>=55){
      high2();
      }
    else{
      digitalWrite(13,LOW);
      }

  //ACS712 reading start 
  
   int adc = analogRead(A4);
   float voltage = (adc*5)/1023.0;
   float current =(2.16 - voltage)/0.100;
   if(current < 0.20){
    current=0;
    }
  //Serial.print("CURRENT-");
  //Serial.println(current);
  lcd.setCursor(0,1);
  lcd.print("Current-");
  lcd.setCursor(8,1);
  lcd.print(current);
  lcd.setCursor(13,1);
  lcd.print("A");
  delay(2000);
  //ACS712 reading end

   
//Sending data to Node MCU
data =String(voltage1)+String(" , ")+String(voltage2)+String(" , ")+String(voltage3)+String(" , ")+String(TX)+String(" , ")+String(current);
espSerial.println(data);
delay(1000);
}

void high1(){
       // Serial.println("Rise in TEMP Detected");
        lcd.setCursor(0,1);
        lcd.print("TEMP RISE");
      for(int i=0;i<=3;i++){
        digitalWrite(13,HIGH);
        delay(200);
        digitalWrite(13,LOW);
        delay(200);}
        lcd.clear();
      }
void high2(){
      lcd.setCursor(0,1);
      lcd.print("HIGH TEMP");
      //Serial.println("HIGH TEMP");
      for(int i=0;i<=6;i++){
        digitalWrite(13,HIGH);
        delay(100);
        digitalWrite(13,LOW);
        delay(100);}
        lcd.clear();
      }
