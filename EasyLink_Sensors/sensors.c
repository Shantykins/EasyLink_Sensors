/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GPRMC_  1
#define _GPGGA_  2
#define _OTHER_  3

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"
#include "MPU6050.h"

//#include "sensors.h"

#define A_CONFIG 0x00
#define G_CONFIG 0x00


typedef struct
{
	float accel[3];
	float gyro[3];
	float temp;
	float lat;
	float lon;
	float alt;

} OutputData;


OutputData payload ;

uint8_t         txBuffer[4];
uint8_t         rxBuffer[4];
uint16_t        calib_accel_XYZ[3];
uint16_t        calib_gyro_XYZ[3];
float           Accel_Data[3];
float           Gyro_Data[3];
float           temperature[1];


/*UART and I2C Handle Definitions */
  I2C_Handle      i2c;
  I2C_Params      i2cParams;

  UART_Handle uart;
  UART_Params uartParams;

int I2C_ReadWrite(I2C_Handle i2c, uint8_t RegAddr, uint8_t *txBuffer , uint8_t *rxBuffer, uint8_t WriteCount, uint8_t ReadCount)
  {
      I2C_Transaction i2cTransaction;

      txBuffer[0] = RegAddr;
      i2cTransaction.slaveAddress = MPU6050_ADDRESS;
      i2cTransaction.writeBuf = txBuffer;
      i2cTransaction.writeCount = WriteCount;
      i2cTransaction.readBuf = rxBuffer;
      i2cTransaction.readCount = ReadCount;

      if (I2C_transfer(i2c, &i2cTransaction)) {
                return 1;
              }
              else {
                  return 0;
              }
  }

int MPU6050_Setup(uint8_t *txBuffer, uint8_t *rxBuffer, uint8_t PWR1_SET , uint8_t PWR2_SET )
{
    //Check Power On Values
        int I2C_Flag = I2C_ReadWrite(i2c, WHO_AM_I_MPU6050, txBuffer, rxBuffer, 1,1);
        I2C_Flag = I2C_ReadWrite(i2c, PWR_MGMT_1, txBuffer, rxBuffer,1,1);

        //Write the Initial PWR Settings
        txBuffer[1] = PWR1_SET;
        I2C_Flag = I2C_ReadWrite(i2c, PWR_MGMT_1, txBuffer, NULL,2,0);
        txBuffer[1] = PWR2_SET;
        I2C_Flag = I2C_ReadWrite(i2c, PWR_MGMT_2, txBuffer, NULL,2,0);

        //Read to see if Value written correctly
        I2C_Flag = I2C_ReadWrite(i2c, PWR_MGMT_1, txBuffer, rxBuffer, 1,1);
        I2C_Flag = I2C_ReadWrite(i2c, PWR_MGMT_2, txBuffer, rxBuffer, 1,1);

        return I2C_Flag;
}

int Gyro_Config(uint8_t *txBuffer, uint8_t *rxBuffer, uint8_t GYRO_SET)
{
    //Write
    txBuffer[1] = GYRO_SET;
    int I2C_Flag = I2C_ReadWrite(i2c, GYRO_CONFIG, txBuffer, NULL,2,0);
    //Read
    I2C_Flag = I2C_ReadWrite(i2c, GYRO_CONFIG, txBuffer, rxBuffer, 1,1);

    return I2C_Flag;
}

int Accel_Config(uint8_t *txBuffer, uint8_t *rxBuffer, uint8_t ACCEL_SET)
{
    //Write
    txBuffer[1] = ACCEL_SET;
    int I2C_Flag = I2C_ReadWrite(i2c, ACCEL_CONFIG, txBuffer, NULL, 2,0);
    //Read
    I2C_Flag = I2C_ReadWrite(i2c, ACCEL_CONFIG, txBuffer, rxBuffer, 1,1);

    return I2C_Flag;
}

void Calibrate(uint8_t *txBuffer, uint16_t *calibXYZ)
{
    //Modify ode to return pointer to array so that there is no local variable issue
    int i =0;
    uint8_t Data[6];
    uint16_t temp;
    calibXYZ[0] =0;
    calibXYZ[1] =0;
    calibXYZ[2] =0;

    for(i = 0;i<50;i++){
                int I2C_Flag = I2C_ReadWrite(i2c, ACCEL_XOUT_H, txBuffer, Data, 1, 6);

                temp = Data[0] << 8 | Data[1];
                calibXYZ[0] = calibXYZ[0] + temp;
                temp = Data[2] << 8 | Data[3];
                calibXYZ[1] = calibXYZ[1] + temp;
                temp = Data[4] << 8 | Data[5];
                calibXYZ[2] = calibXYZ[2] + temp;
        }

    calibXYZ[0] = calibXYZ[0]/50;
    calibXYZ[1] = calibXYZ[1]/50;
    calibXYZ[2] = calibXYZ[2]/50;

    //Add code to check if it is negative and return that value

}

void Read_MPUData()
{
    int i;
    uint8_t Data1[6];
    uint8_t Data2[6];
    uint8_t Data3[2];
    int Accel[3];
    int Gyro[3];
    float divider_a;
    float divider_g;
    uint8_t a = (A_CONFIG & 0x18);
    uint8_t b = (G_CONFIG & 0x18);

    //Set Sensitivity Value For Accelerometer
    switch(a)
    {
    case 0 : divider_a = SSF_AFS_2G; break;
    case 8 : divider_a = SSF_AFS_4G; break;
    case 16 : divider_a = SSF_AFS_8G; break;
    case 24 : divider_a = SSF_AFS_16G; break;
    }

    //Set Sensitivity value for Gyroscope
    switch(b)
     {
     case 0 : divider_g = SSF_GFS_250DPS; break;
     case 8 : divider_g = SSF_GFS_500DPS; break;
     case 16 : divider_g = SSF_GFS_1000DPS; break;
     case 24 : divider_g = SSF_GFS_2000DPS; break;
     }

    int I2C_Flag = I2C_ReadWrite(i2c, ACCEL_XOUT_H, txBuffer, Data1, 1, 6);
    I2C_Flag = I2C_ReadWrite(i2c, GYRO_XOUT_H, txBuffer, Data2, 1, 6);
    I2C_Flag = I2C_ReadWrite(i2c, TEMP_OUT_H, txBuffer, Data3, 1, 2);

    //Convert 8 bit unsigned integer values to 16 bit signed values and subtract the Calibrated readings

    Accel[0] = ((int16_t)(Data1[0] << 8 | Data1[1])) - (int16_t)calib_accel_XYZ[0];
    Accel[1] = ((int16_t)(Data1[2] << 8 | Data1[3])) - (int16_t)calib_accel_XYZ[1];
    Accel[2] = ((int16_t)(Data1[4] << 8 | Data1[5])) - (int16_t)calib_accel_XYZ[2];
    Gyro[0] = ((int16_t)(Data2[0] << 8 | Data2[1])) - (int16_t)calib_gyro_XYZ[0];
    Gyro[1] = ((int16_t)(Data2[2] << 8 | Data2[3])) - (int16_t)calib_gyro_XYZ[1];
    Gyro[2] = ((int16_t)(Data2[4] << 8 | Data2[5])) - (int16_t)calib_gyro_XYZ[2];
    int16_t temporaryTemp = (int16_t)(Data3[0] << 8 | Data3[1]);

  //  tempF[0] = (float)(tempF[0]/340.0) + 36.53;
    payload.temp = (float)(temporaryTemp/340.0) + 36.53; //= tempF[0];

    for(i=0;i<3;i++){
      //  AccelF[i] = (float)(Accel[i]/divider_a);
        payload.accel[i] = (float)(Accel[i]/divider_a);// = AccelF[i];
    //    GyroF[i] = (float)(Gyro[i]/divider_g);
        payload.gyro[i] = (float)(Gyro[i]/divider_g);// = GyroF[i];
    }
}
//---------------------------------------------------------SHANTY EDIT BEGINS--------------------------------------
//temporary global variables due to laziness

uint8_t GPRMC_ok = 0, GPGGA_ok = 0;
uint8_t char_number = 0, SentenceType = 0, Term;
char sentence[6], rawTime[11], rawDate[7], rawSpeed[6], rawCourse[6], rawSatellites[3], rawLatitude[13], rawLongitude[13], rawAltitude[7], buffer[12];

void stringcpy(char *str1, char *str2, uint8_t dir)
{
  uint8_t chr = 0;
  do
  {
    str2[chr + dir] = str1[chr];
  } while(str1[chr++] != '\0');
}

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}



void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

int gps_read()
{
    char temp[1], msg[20] = "CARRIAGE RETURN";
    //int dollar= 0;
    UART_read(uart,&temp, sizeof(temp));
    char c = temp[0];
    if(c == '\r')
        UART_write(uart, &msg, sizeof(msg));
    UART_write(uart, &temp, sizeof(temp));

      switch(c)
      {
      case '\r':  // sentence end
                //UART_read(uart,&temp, sizeof(temp));
          /*
          if(SentenceType == _GPRMC_)
                  GPRMC_ok = 1;
                if(SentenceType == _GPGGA_)
                  GPGGA_ok = 1;
                if(GPRMC_ok && GPGGA_ok) {
                  GPRMC_ok = GPGGA_ok = 0;
                  return 1;
                }
                break; */
          if(SentenceType == _GPGGA_)
          {
             //GPGGA_ok = 1;
             return 1;
          }

      case '$': // sentence start
                Term = char_number = 0;
                break;

              case ',':  // term end (new term start)
                buffer[char_number] = '\0';
                if(Term == 0) {
                  stringcpy(buffer, sentence, 0);
                  if(strcmp(sentence, "GPRMC") == 0)
                    SentenceType = _GPRMC_;
                  else if(strcmp(sentence, "GPGGA") == 0)
                         SentenceType = _GPGGA_;
                       else
                         SentenceType = _OTHER_;
                }

                // Latitude
                if((Term == 2) && (SentenceType == _GPGGA_)) {
                  stringcpy(buffer, rawLatitude, 1);
                }
                // Latitude N/S
                if((Term == 3) && (SentenceType == _GPGGA_)) {
                  if(buffer[0] == 'N')
                    rawLatitude[0] = '0';
                  else
                    rawLatitude[0] = '-';
                }

                // Longitude
                if((Term == 4) && (SentenceType == _GPGGA_)) {
                  stringcpy(buffer, rawLongitude, 1);
                }
                // Longitude E/W
                if((Term == 5) && (SentenceType == _GPGGA_)) {
                  if(buffer[0] == 'E')
                    rawLongitude[0] = '0';
                  else
                    rawLongitude[0] = '-';
                }
                // Altitude
                if((Term == 9) && (SentenceType == _GPGGA_)) {
                  stringcpy(buffer, rawAltitude, 0);
                }
                Term++;
                char_number = 0;
                break;

              default:
                buffer[char_number++] = c;
                break;
            }

            return 0;
}

float parse_rawDegree(char *term_)
{
  float term_value = atof(term_)/100;
  int16_t term_dec = term_value;
  term_value -= term_dec;
  term_value  = term_value * 5/3 + term_dec;
  return term_value;
}

float Latitude()
{
  return parse_rawDegree(rawLatitude);
}

float Longitude()
{
  return parse_rawDegree(rawLongitude);
}

float Altitude()
{
  return atof(rawAltitude);
}


void Read_GPSData()
{

     while(1)
    {
       if(gps_read())
       {
           GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
           payload.lat = Latitude();
           payload.lon = Longitude();
           payload.alt = Altitude();
           return;
       }
       else
       {
           GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_ON);
       }

    }
}
//--------------------------------------------------------SHANTY EDIT ENDS ------------------------------------------
 /*
 *  ======== mainThread ========
 */
void setupSensors()
{
    //I2C Read Write Buffers
    
    float           lat[1], lon[1], alt[1];
    char            msg1[] = "UART Initialized and Ready\r\n";
    char            msg2[] = "Error Initializing I2C\r\n";
    char            msg3[] = "I2C Interface Initialized and Ready\r\n";
    char            msg4[] = "Data Read From Register\r\n";
    char            msg5[] = "I2C Bus Fault\r\n";
    char            msg6[] = "Transaction Completed and I2C Closed\r\n";

    /* Call driver init functions */
    UART_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED and if applicable */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    //Setup UART with Predetermined Settings
    //UART_Setup(uart, uartParams, 9600);
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;

    //Open UART
    uart = UART_open(Board_UART0, &uartParams);
    while(1){
         if (uart == NULL) {
          /* UART_open() failed */
             GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_ON);
           }
           else break;
    }
    //UART_CheckOpen(uart,  uartParams);

    //Print Message to Indicate UART is Initialized
    UART_write(uart,&msg1,sizeof(msg1));

    //Setup I2C with Predetermined Settings
    //I2C_Setup(i2cParams);
    // Create I2C for usage
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;

    //Check if I2C is Null Print. Print Ready if Ready
    //i2c = I2C_CheckOpen(i2c, i2cParams, uart);
    i2c = I2C_open(Board_I2C0, &i2cParams);
     if (i2c == NULL) {
         UART_write(uart,&msg2,sizeof(msg2));
         GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_ON);
      }
       else {
          UART_write(uart, &msg3, sizeof(msg3));
      }

     uint8_t PWR1 = 0x00;
     uint8_t PWR2 = 0x00;
     //Initial Power Management Setting of MPU6050
     int Flag = MPU6050_Setup(txBuffer, rxBuffer, PWR1 , PWR2);

     if(Flag)
     {
         UART_write(uart,&msg4,sizeof(msg4));
     }
     else UART_write(uart,&msg5,sizeof(msg5));

   //  uint8_t G_CONFIG = 0x00;
    //Write the Gyroscope Configuration Settings
    Flag = Gyro_Config(txBuffer, rxBuffer, G_CONFIG);

    //uint8_t A_CONFIG = 0x00;
    //Write Accelerometer Configuration Settings
    Flag = Accel_Config(txBuffer, rxBuffer, A_CONFIG);

    //Calibrate Accelerometer
    Calibrate(txBuffer, calib_accel_XYZ);

    //Calibrate Gyroscope
    Calibrate(txBuffer, calib_gyro_XYZ);
}
//while(1)
 
OutputData updateSensorValues()
{
    //Read Accelerometer, Temperature and Gyroscope Data and Convert to required Scale
    Read_MPUData();

    //Read GPS Data and Extract Required Values.

    Read_GPSData();

    return payload;
}

void closeAll()
{

    char msg6[] = "Transaction Completed and I2C Closed\r\n";
    I2C_close(i2c);
    UART_write(uart, &msg6, sizeof(msg6));
    UART_close(uart);

    return (NULL);
}
