#ifndef _SENSORS_H_
#define _SENSORS_H_


typedef struct OutputData
{
    float accel[3];
    float gyro[3];
    float temp;
    float lat;
    float lon;
    float alt;

} OutputData;


extern void setupSensors(void);


extern OutputData updateSensorValues(void);


extern void closeAll(void);


#endif
