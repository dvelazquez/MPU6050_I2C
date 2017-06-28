/**
* Userspace application for I2C communication
*   Copyright (C) 2013 Andrei Danaila, All Rights Reserved
*<mailto: adanaila >at< ctrlinux[.]com>
*
*	This file is part of i2c_app.
*
* i2c_app is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 2.
*
* i2c_app is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with i2c_app.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "linux/i2c-dev.h"

int main()
{
	int i2c_dev_node = 0;
	int i2c_dev_address = 0x68;  /* Device Address */
	int i2c_dev_reg_addr = 0x6B; /* POWER_CTL Register */
	char i2c_dev_node_path[] = "/dev/i2c-1"; /* Adapter Device Node*/
	unsigned short  i2c_val_to_write = 0x8;
	int ret_val = 0;
	float GForces= 0;
	float Temp= 0;
	__s32 read_value = 0;	
	__s32 read_valueH = 0;
	__s32 read_valueL = 0;

/*	__s16 read_valueT = 0;	
	__s16 read_valueTH = 0;
	__s16 read_valueTL = 0;  */

	int read_valueT = 0;	
	int read_valueTH = 0;
	int read_valueTL = 0;

	int i2c_dev_regH_x_acc = 0x3F; /* Z Hi Measurement Register */
	int i2c_dev_regL_x_acc = 0x40; /* Z Lo Measurement Register */

	int i2c_dev_regH_temp = 0x41; /* Temp Hi Measurement Register */
	int i2c_dev_regL_temp = 0x42; /* Temp Lo Measurement Register */

	/* Open the device node for the I2C adapter of bus 1 */
	i2c_dev_node = open(i2c_dev_node_path, O_RDWR);
	if (i2c_dev_node < 0) {
		perror("Unable to open device node.");
		exit(1);
	}

	/* Set I2C_SLAVE for adapter 1 */
	ret_val = ioctl(i2c_dev_node,I2C_SLAVE,i2c_dev_address);
	if (ret_val < 0) {
		perror("Could not set I2C_SLAVE.");
		exit(2);
	}

	/* Read byte from the 0x2D register of the I2C_SLAVE device */
/*	read_value = i2c_smbus_read_byte_data(i2c_dev_node, i2c_dev_reg_addr);
	if (read_value < 0) {
		perror("I2C Read operation failed.");
		exit(3);
	}
	ret_val = i2c_smbus_write_byte_data(i2c_dev_node,
					i2c_dev_reg_addr,
					i2c_val_to_write);

	if (ret_val < 0) {
		perror("I2C Write Operation failed.");
		exit(4);
	}
*/
	/* Read the measurement from the accelerometer */
	while(1) {
		// AXL
		read_valueH = i2c_smbus_read_byte_data(i2c_dev_node, i2c_dev_regH_x_acc);
		read_valueL = i2c_smbus_read_byte_data(i2c_dev_node, i2c_dev_regL_x_acc);
		// Temp
		read_valueTH = i2c_smbus_read_byte_data(i2c_dev_node, i2c_dev_regH_temp);
		read_valueTL = i2c_smbus_read_byte_data(i2c_dev_node, i2c_dev_regL_temp);

		if (read_valueH < 0) {
			perror("I2C Read operation failed.");
			exit(3);
		}
		//printf("Z Val = %x%x\n", read_valueH, read_valueL);
		read_value= (read_valueH << 8) | read_valueL;

		//printf("Combined = %d\n", read_value);
		GForces=read_value;
		GForces=-(GForces-32768)/16384;
		printf("GForces = %f\n", GForces);

		read_valueT= (read_valueTH << 8) | read_valueTL;
		Temp=read_valueT;
		//Temp= (read_valueT/340)+36.53;		
		//Temp= (((0xFFFF-read_valueT)+1)/154)+24.87;
		Temp= ((Temp-65535)/340)+36.53;
		printf("Temp = %f\n", Temp);

//	sleep(1);

	}
	return 0;
}
