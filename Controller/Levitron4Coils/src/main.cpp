#include <Arduino.h>
#include "PID.h"

PID_s pidZ;
PID_s pidX;
PID_s pidY;

void control_f(int &control, const int pinPWM, const int dir1, const int dir2);


const int PWM_PinX = 9;
const int D_Pin1X = 8;
const int D_Pin2X = 7;


const int PWM_PinY = 10;
const int D_Pin1Y = 6;//11;
const int D_Pin2Y = 5;//12;

char flag = 0;
unsigned int newData = 0;
int prevData = 0;

void setup() {
    // initialize serial communications at 115200 bps:
    Serial.begin(115200);
    pinMode(D_Pin1X, OUTPUT);
    pinMode(D_Pin2X, OUTPUT);
    pinMode(D_Pin1Y, OUTPUT);
    pinMode(D_Pin2Y, OUTPUT);
    pinMode(PWM_PinX, OUTPUT);
    pinMode(PWM_PinY, OUTPUT);

    pidX.k = 1;
    pidX.p = -4.25;
    pidX.i = -0;
    pidX.d = -320;
    pidX.refm = 350;
    pidX.ref = 350;

    pidY.k = 1;
    pidY.p = -4.25;
    pidY.i = -0;
    pidY.d = -320;
    pidY.refm = 350;
    pidY.ref = 350;

    pidZ.k = 1;
    pidZ.p = -4.25;
    pidZ.i = -0;
    pidZ.d = -320;

    cli();//disable interrupts


    //clear ADCSRA and ADCSRB registers
    ADCSRA = 0;
    ADCSRB = 0;
    ADMUX = (1 << REFS1); //set reference voltage

    ADMUX |= (0 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only

    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 2 prescaler- 16mHz/2=8000kHz
    ADCSRA |= (1 << ADATE); //enabble auto trigger
    ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
    ADCSRA |= (1 << ADEN); //enable ADC
    ADCSRA |= (1 << ADSC); //start ADC measurements

    TCCR3A = 0; //  0
    TCCR3B = 0;

    OCR3A = 128; // compare register
    TCCR3B |= (1 << WGM32); // включение в CTC режим

    // 64 prescaler
    TCCR3B |= (1 << CS30);
    TCCR3B |= (1 << CS31);

    TIMSK3 |= (1 << OCIE3A);  // enable interrupt


    TCCR1A = (1 << COM1A1) |(1 << COM1B1)| (1 << WGM10) | (1 << WGM11); //10bit
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // fast pwm 64 prescaler
    TCCR3A = (1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1)|(1<<WGM30);
    TCCR3B = (1<<WGM32) | (1<<CS31);
    sei();//enable interrupts

}

ISR(TIMER3_COMPA_vect) {

    computePID_f(pidX, 1, 0.8, 0.8, 1000);
    computePID_f(pidY, 1, 0.8, 0.8, 1000);
    computePID_f(pidZ, 1, 0.8, 0.8, 1000);
    if (abs(pidZ.diff) > abs(pidX.diff)) {
        control_f(pidZ.control, PWM_PinX, D_Pin1X, D_Pin2X);
    } else {
        control_f(pidX.control, PWM_PinX, D_Pin1X, D_Pin2X);
    }

    if (abs(pidZ.diff) > abs(pidY.diff)) {
        control_f(pidZ.control, PWM_PinY, D_Pin1Y, D_Pin2Y);
    } else {
        control_f(pidY.control, PWM_PinY, D_Pin1Y, D_Pin2Y);
    }

    //control_f(pidZ.control, PWM_PinZ, D_Pin1Z, D_Pin2Z);

}

ISR(ADC_vect) {//when new ADC value ready

    unsigned int lowBitValue;
    switch(flag){
        case 0:
            lowBitValue = ADCL;                 // low bites
            pidX.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
            flag += 1;
            ADMUX = ADMUX & 0b11111000 | flag;
            break;
        case 1:
            lowBitValue = ADCL;                 // low bites
            pidY.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
            flag += 1;
            ADMUX = ADMUX & 0b11111000 | flag;
            break;
        case 2:
            lowBitValue = ADCL;                 // low bites
            pidZ.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
            flag += 1;
            ADMUX = ADMUX & 0b11111000 | flag;
            break;
            break;
        case 3:
            lowBitValue = ADCL;                 // low bites
            pidZ.refm = ((ADCH << 8) | lowBitValue); //concatenation
            flag = 0;
            ADMUX = ADMUX & 0b11111000 | flag;
            break;
        default:
            break;
    }
    lowBitValue = 0;
//    if (flag == 0) {
//
//        unsigned int lowBitValue = ADCL;                 // low bites
//        pidX.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
//        flag += 1;
//        ADMUX = ADMUX & 0b11111000 | flag;
//
//    } else {
//
//        unsigned int lowBitValue = ADCL;                 // low bites
//        pidY.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
//        flag = 0;
//        ADMUX = ADMUX & 0b11111000 | flag;
//    }

}

void loop() {

    // if there's any serial available, read it:

//    if (Serial.available() > 0) {
//
//        // look for the next valid integer in the incoming serial stream:
//
//        pidZ.k = Serial.parseFloat();
//
//        pidZ.p = Serial.parseFloat();
//
//        pidZ.i = Serial.parseFloat();
//
//        pidZ.d = Serial.parseFloat();
//    }
//    // look for the newline. That's the end of the sentence:
//
//    if (Serial.read() == '\n') {

//    Serial.println(pidY.control);
//    Serial.println(pidY.field);
        // Serial.print(pidZ.p);
        Serial.print(pidX.field);
        Serial.print(',');
        Serial.println(pidZ.ref);
//        Serial.print(" ");
//        Serial.println(prevData);
//    Serial.print(',');
        delay(100);
//    }
    Serial.print(pidZ.field);
    Serial.print(',');
    Serial.println(pidZ.ref);
    delay(100);
    //}
}


void control_f(int &control, const int pinPWM, const int dir1, const int dir2) {

    if (control > 1023) { //high border
        control = 1023;
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pinPWM, control);
    } else if ((control >= 0) && (control <= 1023)) {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pinPWM, control);
    } else if ((control < 0) && (control >= (-1023))) { //low border
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
        analogWrite(pinPWM, abs(control));
    } else if (control < -1023) {
        control = -1023;
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
        analogWrite(pinPWM, abs(control));
    }

}




