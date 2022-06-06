#include <Arduino.h>
#include "PID.h"

PID_s pidZ;

void control_f(int &control, const int pinPWM, const int dir1, const int dir2);


const int PWM_PinZ = 6;
const int D_Pin1Z = 8;
const int D_Pin2Z = 7;

char flag = 0;
unsigned int newData = 0;
int prevData = 0;

void setup() {
    // initialize serial communications at 115200 bps:
    Serial.begin(115200);
    pinMode(D_Pin1Z, OUTPUT);
    pinMode(D_Pin2Z, OUTPUT);


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

    ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
    ADCSRA |= (1 << ADATE); //enabble auto trigger
    ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
    ADCSRA |= (1 << ADEN); //enable ADC
    ADCSRA |= (1 << ADSC); //start ADC measurements

    TCCR1A = 0; //  0
    TCCR1B = 0;

    OCR1A = 128; // compare register
    TCCR1B |= (1 << WGM12); // включение в CTC режим

    // 64 prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);

    TIMSK1 |= (1 << OCIE1A);  // enable interrupt


    TCCR3A = (1 << COM3A1) | (1 << WGM30) | (1 << WGM31);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);  // fast pwm

    sei();//enable interrupts

}

ISR(TIMER1_COMPA_vect) {


    computePID_f(pidZ, 1, 0.8, 0.8, 700);
    control_f(pidZ.control, PWM_PinZ, D_Pin1Z, D_Pin2Z);

}

ISR(ADC_vect) {//when new ADC value ready



    if (flag == 0) {

        unsigned int lowBitValue = ADCL;                 // low bites
        pidZ.fieldm = ((ADCH << 8) | lowBitValue); //concatenation
        flag = 1;
        ADMUX = ADMUX & 0b11111000 | flag;

    } else {

        unsigned int lowBitValue = ADCL;                 // low bites
        pidZ.refm = ((ADCH << 8) | lowBitValue); //concatenation
        flag = 0;
        ADMUX = ADMUX & 0b11111000 | flag;
    }

}

void loop() {

    // if there's any serial available, read it:

    if (Serial.available() > 0) {

        // look for the next valid integer in the incoming serial stream:

        pidZ.k = Serial.parseFloat();

        pidZ.p = Serial.parseFloat();

        pidZ.i = Serial.parseFloat();

        pidZ.d = Serial.parseFloat();
    }
    // look for the newline. That's the end of the sentence:

    if (Serial.read() == '\n') {

//    Serial.println(pidY.control);
//    Serial.println(pidY.field);
        // Serial.print(pidZ.p);
        Serial.print(pidZ.field);
        Serial.print(',');
        Serial.println(pidZ.ref);
//        Serial.print(" ");
//        Serial.println(prevData);
//    Serial.print(',');
        delay(100);
    }
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




