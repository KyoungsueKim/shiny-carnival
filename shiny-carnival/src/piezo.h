#include <vector>

typedef enum Scale
{
    C = 261,
    D = 293,
    E = 329,
    F = 349,
    G = 391,
    A = 440,
    B = 493,
    highC = 523
} Scale;

class Melody {
    public:
        std::vector<int> melody = { C, D, E, F, G, A, B, highC };

        /*** 
        * Melody 오브젝트의 melody 변수에 기록된 멜로디에 따라 음을 재생합니다. 
        * @return : NULL 포인터
        */
        void *playMelody();

        /***
         * 피에조의 음을 재생합니다. 음계와 지속시간을 설정해줄 수 있습니다.
         * @param scale : 재생을 원하는 음계 (Hz 단위)
         * @param duration : 지속 시간
         */
        void *piezo(int scale, float duration);
};