#include "camera.h"

enum Sensor{
    SENSOR_1 = 6,
    SENSOR_2 = 23,
    SENSOR_3 = 25,
    SENSOR_4 = 25,
    SENSOR_5 = 23,
    SENSOR_6 = 6
};

enum Mode{
    MODE_1 = 0,
    MODE_2 = 1
};

const char* SensingWithEncoded(int mode);
Directions DecodeSensorData(const char* encodedData1, const char* encodedData2);