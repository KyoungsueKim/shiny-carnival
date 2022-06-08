#include "camera.h"

// TODO: 4, 5, 6번 센서 몇번 핀인지 기입
enum Sensor{
    SENSOR_1 = 6,
    SENSOR_2 = 23,
    SENSOR_3 = 25,
    SENSOR_4 = 0,
    SENSOR_5 = 0,
    SENSOR_6 = 0
};

enum Mode{
    MODE_1 = 0,
    MODE_2 = 1
};

static const char* SensingWithEncoded(int mode);
static Directions DecodeSensorData(const char* encodedData1, const char* encodedData2);