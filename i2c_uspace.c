/*
  MPU6050 Interface
  Based on: Userspace application for I2C communication

  Daniel Velazquez
  Oct 2016
*/
#include <time.h>    //Time
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "linux/i2c-dev.h"

double time_diff(struct timeval x , struct timeval y);

int main()
{
	char FileName[70]="FileTest.txt";
   char c[1000];
	char quit;
//	FileName="FileTest.txt";
//	strcpy(FileName,"FileTest.txt");
	FILE *Log;
   Log= fopen(FileName,"w");

	struct timeval before , after;

	int i2c_dev1_node = 0;
	int i2c_dev2_node = 0;
	int i2c_dev1_address = 0x68;  /* Device Address 01 */
	int i2c_dev2_address = 0x69;  /* Device Address 02 */
	int i2c_dev_reg_addr = 0x6B; /* POWER_CTL Register */
	char i2c_dev_node_path[] = "/dev/i2c-1"; /* Adapter Device Node*/

	int ret_val = 0;
	int i;
	/* Initialize all empty */
	float GForcesX= 0;
	float GForcesY= 0;
	float GForcesZ= 0;
	float Temp= 0;
	float GyroX= 0;
	float GyroY= 0;
	float GyroZ= 0;
	/* wildcard to be used on all H and L registers */
	int Read_Value_High = 0;
	int Read_Value_Low = 0;
	int Read_Value;

	int MPU6050_dev1_Register_[16];	//this one contains all MPU6050 Registers for Dev1
	int MPU6050_dev2_Register_[16];	//this one contains all MPU6050 Registers for Dev2
	/* 0x3B= AccX_H, 0x3C= AccX_L			59-60
		 0x3D= AccY_H, 0x3E= AccY_L			61-62
		 0x3F= AccZ_H, 0x40= AccZ_L			63-64
		 0x41= Temp_H, 0x42= Temp_L		65-66
		 0x41= Temp_H, 0x42= Temp_L		67-68
		 0x43= GyroX_H, 0x44= GyroX_L		69-70
		 0x45= GyroY_H, 0x46= GyroY_L		71-72
		 0x47= GyroZ_H, 0x48= GyroS_L 	73-74		*/
	// With something like  MPU6050_Registers_[i];

	/* Open the device node for the I2C adapter of bus 1 */
	i2c_dev1_node = open(i2c_dev_node_path, O_RDWR);
	if (i2c_dev1_node < 0) {
		perror("Unable to open device node.");
		exit(1);
	}
	/* Open the device node for the I2C adapter of bus 2 */
	i2c_dev2_node = open(i2c_dev_node_path, O_RDWR);
	if (i2c_dev2_node < 0) {
		perror("Unable to open device node.");
		exit(1);
	}
	/* Set I2C_SLAVE for Sensor 1 */
	ret_val = ioctl(i2c_dev1_node,I2C_SLAVE,i2c_dev1_address);
	if (ret_val < 0) {
		perror("Could not set I2C_SLAVE.");
		exit(2);
	}
	/* Set I2C_SLAVE for Sensor 2 */
	ret_val = ioctl(i2c_dev2_node,I2C_SLAVE,i2c_dev2_address);
	if (ret_val < 0) {
		perror("Could not set I2C_SLAVE.");
		exit(2);
	}

	fprintf(Log,"us diff,GForcesX_1,GForcesY_1,GForcesZ_1,Temp1,GForcesX_2,GForcesY_2,GForcesZ_2,Temp2\n");
	/* Read ALL measurement from the MPU6050 */
//	while(quit!='q') {
	while(1){
    gettimeofday(&before , NULL);
		// Read All Values
		for(i=59; i<=72;i++){
			MPU6050_dev1_Register_[i] = i2c_smbus_read_byte_data(i2c_dev1_node, i);
			MPU6050_dev2_Register_[i] = i2c_smbus_read_byte_data(i2c_dev2_node, i);
	}

		if (MPU6050_dev1_Register_[59] < 0) {		// Check if there is any data
			perror("I2C _Read operation failed.");
			exit(3);
		}
		GForcesX=(MPU6050_dev1_Register_[59] << 8) | MPU6050_dev1_Register_[60];
		GForcesX=-(GForcesX-32768)/16384;
		printf("GForcesX Dev1 = %f\n", GForcesX);
		fprintf(Log,"%f,",GForcesX);

		GForcesY=(MPU6050_dev1_Register_[61] << 8) | MPU6050_dev1_Register_[62];
		GForcesY=-(GForcesY-32768)/16384;
		printf("GForcesY Dev1= %f\n", GForcesY);
		fprintf(Log,"%f,",GForcesY);

		GForcesZ=(MPU6050_dev1_Register_[63] << 8) | MPU6050_dev1_Register_[64];
		GForcesZ=-(GForcesZ-32768)/16384;
		printf("GForcesZ Dev1= %f\n", GForcesZ);
		fprintf(Log,"%f,",GForcesZ);

		Temp= (MPU6050_dev1_Register_[65] << 8) | MPU6050_dev1_Register_[66]; 
		Temp= ((Temp-65535)/340)+36.53;
		printf("Temp Dev1= %f\n", Temp);
		fprintf(Log,"%f,",Temp);

		GForcesX=(MPU6050_dev2_Register_[59] << 8) | MPU6050_dev2_Register_[60];
		GForcesX=-(GForcesX-32768)/16384;
		printf("GForcesX Dev2 = %f\n", GForcesX);
		fprintf(Log,"%f,",GForcesX);

		GForcesY=(MPU6050_dev2_Register_[61] << 8) | MPU6050_dev2_Register_[62];
		GForcesY=-(GForcesY-32768)/16384;
		printf("GForcesY Dev2= %f\n", GForcesY);
		fprintf(Log,"%f,",GForcesY);

		GForcesZ=(MPU6050_dev2_Register_[63] << 8) | MPU6050_dev2_Register_[64];
		GForcesZ=-(GForcesZ-32768)/16384;
		printf("GForcesZ Dev2= %f\n", GForcesZ);
		fprintf(Log,"%f,",GForcesZ);

		Temp= (MPU6050_dev2_Register_[65] << 8) | MPU6050_dev2_Register_[66]; 
		Temp= ((Temp-65535)/340)+36.53;
		printf("Temp Dev2= %f\n", Temp);
		fprintf(Log,"%f\n",Temp);

	gettimeofday(&after , NULL);
   fprintf(Log,"%.0lf," , time_diff(before , after) ); 
	system("clear");

//	quit=getchar();

//	sleep(1);

	}
   fclose(Log);
	return 0;
}

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}
