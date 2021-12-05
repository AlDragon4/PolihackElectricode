#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//temperature pin
const int pin_temp = A0;
float temperatura = 0.00;
float media = 0.00;

//photoresistor pin
const int pin_phres = A1;
int phres_val = 0;

//switch pin
const int pin_switch = 13;
int switchst = LOW;

//ultrasonic distance sensor pinout
const int trig_Pin = 6;
const int echo_Pin = 7;
long duration = 0.0;
long distance = 0.0;

//rgb led pinout
const int redl = 10;
const int bluel = 9;
const int greenl = 8;

//display val
int Displ = 0;
int N = 500;

//triggers
byte window_trig = false;
byte switch_trig = false;
byte temp_light_trig = false;

void setup()
{
  Serial.begin(9600);
  
  pinMode(trig_Pin, OUTPUT);
  pinMode(echo_Pin, INPUT);
  
  pinMode(pin_switch, INPUT);

  pinMode(redl, OUTPUT);
  pinMode(bluel, OUTPUT);
  pinMode(greenl, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void setColor(int redValue, int greenValue, int blueValue)
{
  if(switch_trig == false)
  {
 	analogWrite(redl, redValue);
 	analogWrite(greenl, greenValue);
 	analogWrite(bluel, blueValue);
  }
}

void ultraSense()
{
  //Serial.println("Starting Trigger");
  digitalWrite(trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Pin, LOW);
  duration = pulseIn(echo_Pin, HIGH);
  
  distance = duration * 0.0343 / 2.0;
  Serial.println("\nDistance(cm): ");
  Serial.println(distance);
  
  if(distance < 40)
  {
  	setColor(0, 0, 255);
  }
  else if(distance > 40)
    setColor(0, 0, 0);
  
}

void displayTemp()
{
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Temperatura [*C]");
 float suma = 0.00;
  
   for(int i=1; i <= N; i++)
    {
      Displ = analogRead(pin_temp);
      temperatura = (((Displ / 1024.0) * 5)- 0.5)*100;
      suma += temperatura;
    }
    media = suma / N;
    lcd.setCursor(0, 1);
    if(media < 1000)
    {
      lcd.setCursor(3, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(media);
    }
    if(media < 100)
    {
      lcd.setCursor(2, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(media);
    }
    if(media < 10)
    {
      lcd.setCursor(1, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(media);
    }
  delay(3000);
}

double LightIntensityCalculator(int AVal)
{
  int DResistorVal = 3.3;
  double vout = AVal * 0.0048828125;
  int luxData = ((2500/vout-500)/DResistorVal);
  return luxData;
}

void displayLight()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminozitate");
  phres_val = analogRead(pin_phres);
  
  Serial.println("\nLight intensity(lux): ");
  //Serial.println(LightIntensityCalculator(phres_val));
  int luxValue = LightIntensityCalculator(phres_val);
  Serial.println(luxValue);
  
  if(luxValue > 7000)
  {
  	lcd.setCursor(0, 1);
  	lcd.print("Luminos");
    setColor(255, 0, 0);
  }
  else if(luxValue > 700)
  {
  	lcd.setCursor(0, 1);
  	lcd.print("Innorat");
    setColor(255, 0, 0);
  }
  else 
  {
  	lcd.setCursor(0, 1);
  	lcd.print("Intunecat");
    setColor(0, 0, 0);
  }
  
  delay(2000);
}

void TestingSensors()
{
  Serial.println("\nSwitch val: ");
  Serial.println(switchst);
}

void loop()
{
  Serial.println("----------------------");
  //digital read for soft override
  switchst = digitalRead(pin_switch);
  Serial.println(switchst);
  
  if(switchst == 1)
    switch_trig = true;
  else
    switch_trig = false;
  
  if(temp_light_trig == false)
  {
    displayTemp();
    temp_light_trig = true;
  }
  else
  {
    displayLight();
    temp_light_trig = false;
  }
  
  ultraSense();
  
  
  //TestingSensors();
  delay(500);
}