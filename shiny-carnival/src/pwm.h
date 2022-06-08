#define IN 0
#define OUT 1

#define IN 0
#define LOW 0

#define VALUE_MAX 45
#define DIRECTION_MAX 45

int PWMExport(int pwmnum);
int PWMEnable(int pwmnum);
int PWMWritePeriod(int pwmnum, int value);
int PWMWriteDutyCycle(int pwmnum, int value);