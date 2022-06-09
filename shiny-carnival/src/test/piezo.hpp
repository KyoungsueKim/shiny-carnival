#ifndef PIEZO_H
# define PIEZO_H

#include <vector>

typedef enum Scale
{
    ZERO = 0,
    lowC = 262,
    lowCsharp = 277,
    lowD = 294,
    lowDsharp = 311,
    lowE = 330,
    lowF = 349,
    lowFsharp = 370,
    lowG = 392,
    lowGsharp = 415,
    lowA = 440,
    lowAsharp = 466,
    lowB = 494,
    C = 523,
    Csharp = 554,
    D = 587,
    Dsharp = 622,
    E = 659,
    F = 698,
    Fsharp = 740,
    G = 784,
    Gsharp = 831,
    A = 880,
    Asharp = 932,
    B = 988,
    highC = 1047
} Scale;

/***
 * Usage: 
 *      Melody basic = Melody();
        basic.playMelody(0.3);

        Melody airplain = Melody();
        airplain.melody = {E, D, C, D, E, E, E, ZERO, D, D, D, ZERO, E, G, G, ZERO, E, D, C, D, E, E, E, ZERO, D, D, E, D, C};
        airplain.playMelody(0.3);

        Melody warning = Melody();
        warning.melody = {E, Dsharp, E, Dsharp, E, lowB, D, C, lowA, ZERO, ZERO, lowC, lowE, lowA, lowB, ZERO, lowE, lowG, lowB, C, ZERO, ZERO, E, Dsharp, E, Dsharp, E, lowB, D, C, lowA, ZERO, ZERO, lowC, lowE, lowA, lowB, ZERO, lowE, C, lowB, lowA, ZERO, ZERO};
        warning.playMelody(0.3);
 */
class Melody {
    public:
        std::vector<int> melody = { C, D, E, F, G, A, B, highC };

        /*** 
        * Melody 오브젝트의 melody 변수에 기록된 멜로디에 따라 음을 재생합니다. 
        * @return : NULL 포인터
        */
        void *playMelody(float duration);

        /***
         * 피에조의 음을 재생합니다. 음계와 지속시간을 설정해줄 수 있습니다.
         * @param scale : 재생을 원하는 음계 (Hz 단위)
         * @param duration : 지속 시간
         */
        static void *piezo(int scale, float duration);
};

#endif