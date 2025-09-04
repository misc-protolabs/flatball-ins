// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "util.h"

float filt_1ord( float x, float y_z1, float tau, float dt)
{
  float a0, y;
  if( tau <= 0 || dt <= 0)
  {
    y = x;
  }
  else
  {
    a0 = dt / tau;
    y = (a0 * (x - y_z1)) + y_z1;
  }
  return y;
}

  //boot_btn = edge_dbnc_rise( boot_btn, 0.25, &boot_btn_tmr, &boot_btn_x_z1, &boot_btn_y_z1);
bool edge_dbnc_rise(bool x, float t_dbnc, float* tmr_z1, bool* x_z1, bool* y_z1)
{
  bool y;

  if ((x != *x_z1) && x) {
    *tmr_z1 = t_dbnc;
  } else {
    *tmr_z1 -= 0.01;
  }

  if (*tmr_z1 <= 0.0) {
    *tmr_z1 = 0.0;
  }

  *x_z1 = *y_z1;

  if (*tmr_z1 == 0.0) {
    y = x;
  } else {
    y = *x_z1;
  }

  *x_z1 = x;
  *y_z1 = y;
  return y;
}
