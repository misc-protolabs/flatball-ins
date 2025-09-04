% Copyright 2025 Michael V. Schaefer
% 
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at:
% 
%     http://www.apache.org/licenses/LICENSE-2.0
% 
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.


%% control calibrations / parameters

%% launch timing calibrations
k_launch_F_t_release = 0.60;
k_launch_M_t_release = 0.60;

k_launch_Fx_t_launch_begin = 0.50;
k_launch_Fx_t_launch_end = k_launch_F_t_release;
k_launch_Fy_t_launch_begin = 0.50;
k_launch_Fy_t_launch_end = k_launch_F_t_release;
k_launch_Fz_t_launch_begin = 0.10;
k_launch_Fz_t_launch_end = k_launch_F_t_release;

k_launch_Mx_t_launch_begin = 0.30;
k_launch_Mx_t_launch_end = k_launch_M_t_release;
k_launch_My_t_launch_begin = 0.40;
k_launch_My_t_launch_end = k_launch_M_t_release;
k_launch_Mz_t_launch_begin = 0.20;
k_launch_Mz_t_launch_end = k_launch_M_t_release;

%% launch force calibrations
k_launch_Fx_tau = 1e-2;
k_launch_Fx_kp = 7.5;
k_launch_Fx_ki = 15;
k_launch_Fx_kd = 2e-2;
k_launch_Fx_sat_hi = 150;
k_launch_Fx_sat_lo = -150;

k_launch_Fy_tau = 1e-2;
k_launch_Fy_kp = k_launch_Fx_kp;
k_launch_Fy_ki = k_launch_Fx_ki;
k_launch_Fy_kd = k_launch_Fx_kd;
k_launch_Fy_sat_hi = k_launch_Fx_sat_hi;
k_launch_Fy_sat_lo = k_launch_Fx_sat_lo;

k_launch_Fz_tau = 1e-2;
k_launch_Fz_kp = 20;
k_launch_Fz_ki = 50;
k_launch_Fz_kd = 2.5;
k_launch_Fz_sat_hi = 25;
k_launch_Fz_sat_lo = -25;

%% launch moment calibrations
k_launch_Mx_tau = 1e-2;
k_launch_Mx_kp = 3.5;
k_launch_Mx_ki = 1.125;
k_launch_Mx_kd = 0.125;
k_launch_Mx_sat_hi = 2.5;
k_launch_Mx_sat_lo = -2.5;

k_launch_My_tau = 1e-2;
k_launch_My_kp = 4.25;
k_launch_My_ki = 5.5;
k_launch_My_kd = 0.1125;
k_launch_My_sat_hi = 2.5;
k_launch_My_sat_lo = -2.5;

k_launch_Mz_tau = 1e-2;
k_launch_Mz_kp = 0.25;
k_launch_Mz_ki = 0.2575;
k_launch_Mz_kd = 1e-8;
k_launch_Mz_sat_hi = 7.5;
k_launch_Mz_sat_lo = -7.5;
