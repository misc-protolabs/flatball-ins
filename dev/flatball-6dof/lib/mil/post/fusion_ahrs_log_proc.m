# Copyright 2025 Michael V. Schaefer
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

%%
flatball_init_disc_params;

fldr_exp = '.\'; %'.\wrk\f-dev-3.1.3-datalogs\2024\07-oct';
n_exp = 'x0000001a';

% fldr_exp = '.\wrk\f-dev-3.1.4-datalogs\2024\01-oct';
% n_exp = 'x00000011';

fname = [ fldr_exp '\' n_exp '.csv'];

tsc = flatball_log_proc( fname);

%%
yaw_dt2 = ts_derrivative( tsc.gyro_z);
idx_w_pk = find( tsc.gyro_z.Data >= tsc.gyro_z.max, 1);
idx_w_crash = find( yaw_dt2.Data <= yaw_dt2.min, 1);

v_fl = ts_integrate_win( tsc.fl_acc, idx_w_pk, idx_w_crash);

x0 = 0;
y0 = 0;
z0 = 1;
u0 = v_fl.max;
v0 = 0;
w0 = 0;

phi0 = tsc.roll.Data(idx_w_pk) .* (pi/180);
theta0 = tsc.pitch.Data(idx_w_pk) .* (pi/180);
psi0 = tsc.yaw.Data(idx_w_pk) .* (pi/180) .* 0;

phidot0 = tsc.gyro_x.Data(idx_w_pk) .* (2*pi/360);
thetadot0 = tsc.gyro_y.Data(idx_w_pk) .* (2*pi/360);
psidot0 = tsc.gyro_z.Data(idx_w_pk) .* (2*pi/360);

[x0 y0 z0 u0 v0 w0 phi0 theta0 psi0 phidot0 thetadot0 psidot0]

%%
flatball_log_plots;

%%
acc_x = tsc.acc_x .* 9.81;
acc_y = tsc.acc_y .* 9.81;
acc_z = tsc.acc_z .* 9.81;
acc = acc_x.*acc_x + acc_y.*acc_y + acc_z.*acc_z;
acc.Data = acc.Data .^ 0.5;

sign_acc_x = tsc.acc_x; sign_acc_x.Data = sign( tsc.acc_x.Data);
sign_acc_y = tsc.acc_y; sign_acc_y.Data = sign( tsc.acc_y.Data);
sign_w = tsc.gyro_z; sign_w.Data = sign( tsc.gyro_z.Data);

%% gyro installation offset
r_x = 19.5e-3;
r_y = 7.5e-3;

acc_x_centr = tsc.gyro_z .* tsc.gyro_z .* (pi/180)^2 .* r_x .* sign_acc_x;
acc_y_centr = tsc.gyro_z .* tsc.gyro_z .* (pi/180)^2 .* r_y .* sign_acc_y;

acc_x_new = acc_x - acc_x_centr;
acc_y_new = acc_y - acc_y_centr;
acc_z_new = acc_z;
acc_new = acc_x_new.*acc_x_new + acc_y_new.*acc_y_new - acc_z_new.*acc_z_new;
acc_new.Data = acc_new.Data .^ 0.5;

win_len = 100;
vx = ts_integrate_win( acc_x_new, idx_w_pk-win_len, idx_w_pk);
vy = ts_integrate_win( acc_y_new, idx_w_pk-win_len, idx_w_pk);
vz = ts_integrate_win( acc_z, idx_w_pk-win_len, idx_w_pk) .* 0;
v = vx.*vx + vy.*vy + vz.*vz;
v.Data = v.Data .^ 0.5;

%%
new_formatted_fig( 'tmp');
subplot(211);
hold on;
acc_x.plot;
acc_x_centr.plot;
acc_x_new.plot;
hold off;
subplot(212);
hold on;
acc_y.plot;
acc_y_centr.plot;
acc_y_new.plot;
hold off;

%%
new_formatted_fig( 'tmp2');
subplot(211);
hold on;
acc_x.plot;
acc_y.plot;
hold off;
subplot(212);
hold on;
acc_x_new.plot;
acc_y_new.plot;
hold off;

new_formatted_fig( 'tmp3');
subplot(211);
hold on;
vx.plot;
vy.plot;
vz.plot;
hold off;

subplot(212);
v.plot;

