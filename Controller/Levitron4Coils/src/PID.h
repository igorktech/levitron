//
// Created by ik76 on 21.05.2022.
//

#ifndef LEVITRON_PID_H
#define LEVITRON_PID_H


struct PID_s {
    float field = 0;       // значение с датчика Холла
    float ref = 0; // Задание (потенциометр на третьей ноге)
    float fieldm = 0;       // значение с датчика Холла
    float refm = 0; // Задание (потенциометр на третьей ноге)
    float error = 0;     // Ошибка управления
    int control = 0;     // Сигнал управления
    float integral = 0;    // интегральная составляющая
    float diff = 0;
    float prevField = 0;
    float prevRef = 0;
    float prevError = 0;
    float k = 0;
    float p = 0;
    float i = 0;
    float d = 0;
};



void computePID_f(PID_s &pid, float deltaTime, float kRefFilter, float kFieldFilter, unsigned int border) {
    //обнуление сигнала управления
    pid.control = 0;

    pid.error = pid.ref - pid.field; // ошибка управления
    //фильтрация
    pid.field = pid.prevField * (1 - kFieldFilter) + kFieldFilter * pid.fieldm;
    pid.ref = pid.prevRef * (1 - kRefFilter) + kRefFilter * pid.refm;

    if (pid.ref >= border) { //ограничение
        pid.ref = border;
    }

    pid.diff = (pid.error - pid.prevError) / deltaTime; // находим производную
    pid.integral += (pid.error - pid.prevError) * deltaTime;   // находим интегал
    //
    pid.prevField = pid.field;
    pid.prevRef = pid.ref;
    pid.prevError = pid.error;

    //сигнал управления ПИД-регулятора
    pid.control = pid.k * (pid.p * pid.error + pid.i * pid.integral + pid.d * pid.diff);
}

#endif //LEVITRON_PID_H
