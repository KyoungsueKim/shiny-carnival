#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "pwm.h"
#include "piezo.h"

void* Melody::playMelody(){
    // PWM 0 Initialize
    if (PWMExport(0) == 0 && PWMEnable(0) == 0) {
        // play
        for (int i = 0; i < melody.size(); i++) {
            piezo(melody[i], 1);
        }

        PWMWriteDutyCycle(0, 0); // mute
    }

    return NULL;
}

void* Melody::piezo(int scale, float duration){

    PWMWritePeriod(0, (1000000 / scale) * 1000);
    PWMWriteDutyCycle(0, ((1000000 / 2) / 261));
    usleep((int)(1000000 * duration));

    return NULL; 
}