#ifndef _APP_H_
#define _APP_H_

extern bool app_init(void);
extern void app_step_100Hz(void);
extern void app_step_10Hz(void);
extern void app_step_1Hz(void);

enum fdevFlightState
{
    IDLE,
    LAUNCH,
    IN_FLIGHT,
    FLIGHT_COMPLETE
};

extern float gx, gy, gz;
extern float ax, ay, az;
extern float mx, my, mz;
extern float qw, qx, qy, qz;
extern float pitch, roll, yaw;
extern float fe_ax, fe_ay, fe_az;
extern float fl_ax, fl_ay, fl_az;
extern float degC, patm;
extern float v_batt, soc, soc_filt; //, soc_rate_of_change
extern unsigned long tstamp;
extern int fdev_st, fdev_flight_st;
extern int fdev_st_z1, fdev_flight_st_z1;
extern float dt_fusion;
extern float patm_filt, degC_filt;
extern float acc_filt, gz_filt;

// fusion internals
extern float ahrs_accel_err;
extern float ahrs_accel_recovery_trig;
// extern bool ahrs_accel_ignored;
extern float ahrs_mag_err;
extern float ahrs_mag_recovery_trig;
// extern bool ahrs_mag_ignored;
extern char ahrs_flgs;

#endif
