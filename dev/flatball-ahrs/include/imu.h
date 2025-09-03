#ifndef _IMU_H_
#define _IMU_H_

extern void imu_init( void);
extern void imu_step( float*, float*, float*, float*, float*, float*);
extern void get_imu_temp( float*);

//extern SparkFun_ISM330DHCX_SPI imu6dof; 

#endif
