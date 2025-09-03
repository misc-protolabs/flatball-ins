#ifndef _UTIL_H_
#define _UTIL_H_

extern float filt_1ord( float x, float y_z1, float tau, float dt);
extern bool edge_dbnc_rise(bool x, float t_dbnc, float* tmr_z1, bool* x_z1, bool* y_z1);

#endif
