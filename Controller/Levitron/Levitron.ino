/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin0 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin1 = A2
;
const int PinPWM = 10;
const int Pin1 = 11;
const int Pin2 = 12;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

int field;       // значение с датчика Холла
int ref; // Задание (потенциометр на третьей ноге)
int error = 0;     // Ошибка управления
int control;     // Сигнал управления
int integral;    // интегральная составляющая
int diff = 0;
int prevField = 0;

float k;
float p;
float i;
float d;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  pinMode(Pin1,OUTPUT);
  pinMode(Pin2,OUTPUT);
}

void loop() {
  // if there's any serial available, read it:

// while (Serial.available() > 0) {
//
//    // look for the next valid integer in the incoming serial stream:
//
//     k = Serial.parseFloat();
//
//    // do it again:
//
//   p = Serial.parseFloat();
//
//    // do it again:
//
//    i = Serial.parseFloat();
//    
//     d = Serial.parseFloat();
//    // look for the newline. That's the end of your sentence:
//
//    if (Serial.read() == '\n') {


  
  ref = (analogRead(analogInPin1) + analogRead(analogInPin1) +analogRead(analogInPin1) + analogRead(analogInPin1) + analogRead(analogInPin1)) / 5;
   
    if (ref >= 300)
    { //ограничение задани€
     ref = 500;
    }
    field = (analogRead(analogInPin0) + analogRead(analogInPin0) +analogRead(analogInPin0) + analogRead(analogInPin0) + analogRead(analogInPin0)) ; //значение с датчика Холла
      diff = prevField - field; // находим разницу с предыдущим значением (Дифференциальная составляющая)
      prevField = field;
    error = ref - field; // ошибка управления для 8 бит ацп
    integral += error;   // чем больше ки тем больше перерегулирование
               //сигнал управления ПИД-регулятора
    control = error*(-2) + diff *(-70)+integral/(500);//+integral/(-1000);//0 + error*(-600) + diff *(-500)-integral/100;
// control = -1023;
//    Serial.print(y1);
//  Serial.print(" "); // a space ' ' or  tab '\t' character is printed between the two values.
  //Serial.print(field);
  //Serial.print(" "); // a space ' ' or  tab '\t' character is printed between the two values.
  //Serial.print(ref);
  //Serial.print(" "); // a space ' ' or  tab '\t' character is printed between the two values.
  // the last value is followed by a carriage return and a newline characters.
    if (control > 1023)
    { //ограничение сигнала управления сверху
      control = 1023;
       digitalWrite(Pin1,HIGH);
      digitalWrite(Pin2,LOW);
      analogWrite(PinPWM,control);
     
    }
    else if((control>=0)&&(control<=1023)){
      digitalWrite(Pin1,HIGH);
    digitalWrite(Pin2,LOW);
    analogWrite(PinPWM,control);
    
    }
   else if ((control < 0)&&(control>=-1023))
    { //ограничение сигнала управления снизу
       digitalWrite(Pin1,LOW);
        digitalWrite(Pin2,HIGH);
      analogWrite(PinPWM,abs(control));
      
    }
   else if (control < -1023)
    { //ограничение сигнала управления сверху
      control = -1023;
       digitalWrite(Pin1,LOW);
      digitalWrite(Pin2,HIGH);
       analogWrite(PinPWM,abs(control));
       
    }

     //}
//            Serial.print("k");
//     Serial.println(35);
//    Serial.print("p");
// Serial.println(p);
//     Serial.print("i");
//     Serial.println(i);
//     Serial.print("d");
//     Serial.println(d);
// }
//            Serial.print("k");
  //   Serial.println(k);
//    Serial.print("p");
    Serial.println(control);
//     Serial.print("i");
//     Serial.println(i);
//     Serial.print("d");
//     Serial.println(d);
//analogWrite(PinPWM0,control);
 // analogWrite(PinPWM1,control);
}
