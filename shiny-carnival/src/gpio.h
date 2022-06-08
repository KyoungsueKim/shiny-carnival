#define IN 0
#define OUT 1

#define VALUE_MAX 45
#define DIRECTION_MAX 45

int GPIOExport(int pin);
int GPIOUnexport(int pin);
int GPIODirection(int pin, int dir);
int GPIORead(int pin);
int GPIOWrite(int pin, int value);