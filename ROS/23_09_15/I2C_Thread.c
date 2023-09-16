#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_Device "/dev/i2c-0"
#define Slave_Addess 0x05
#define Data_Length 8

uint8_t SendData[Data_Length];
uint8_t ReceivedData[Data_Length];
pthread_mutex_t i2cMutex1;

int I2C_Mod;
long EncoderPos=0;
void* I2C_Thread(void* arg) 
{
  while (1) 
  {   
    pthread_mutex_lock(&i2cMutex1);
    if (write(I2C_Mod, SendData, sizeof(SendData)) != sizeof(SendData)) 
    {
        perror("Write Failed");
    }
    else printf("Send\n");

    pthread_mutex_unlock(&i2cMutex1); 
    pthread_mutex_lock(&i2cMutex1);
	read(I2C_Mod, ReceivedData,Data_Length);
    pthread_mutex_unlock(&i2cMutex1);
    
	EncoderPos=ReceivedData[3]<<24|ReceivedData[4]<<16|ReceivedData[5]<<8|ReceivedData[6];
	printf("encoder : %d",EncoderPos);

	printf("\n");
    usleep(100000);
  }
}


int main()
{
	long Second=0;
	
	for(uint8_t i =0;i<Data_Length;i++)
	{
		SendData[i]=0;
	}
	
    I2C_Mod = open(I2C_Device, O_RDWR);
    if (I2C_Mod < 0)
    {
        perror("Unable to open I2C device");
        return 1;
    }
    if (ioctl(I2C_Mod, I2C_SLAVE, Slave_Addess) < 0) 
    {
        perror("Failed to acquire bus access and/or talk to slave");
        return 1;
    }
    
	pthread_mutex_init(&i2cMutex1, NULL);
	pthread_t pthread_Encoder_Value;
	printf("Create I2C_Thread \n");
	pthread_create(&pthread_Encoder_Value,NULL,I2C_Thread,NULL);
	
	while(1)
	{
		printf("\nTime : %3d \n",Second);
		Second++;
		SendData[0]= (EncoderPos&0xff000000)>>24;
		SendData[1]= (EncoderPos&0x00ff0000)>>16;
		SendData[2]= (EncoderPos&0xff00)>>8;
		SendData[3]= (EncoderPos&0x00ff); 
		SendData[4]= (Second&0xff000000)>>24;    // encoder MSB 8bit
		SendData[5]= (Second&0x00ff0000)>>16;
		SendData[6]= (Second&0x0000ff00)>>8;
		SendData[7]= (Second&0x000000ff);        // encoder LSB 8bit
		sleep(1);
	}
	close(I2C_Mod);
    pthread_mutex_destroy(&i2cMutex1);
	return 1;
}
