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

function flatball_log_plots( tsc)
%%
C = evalin( 'base', 'C');

[idx_launch_beg, idx_launch_end, idx_impact, ccw] = flatball_log_find_flight_window( tsc);

[x0, y0, z0, ...
 u0, v0, w0, ...
 phi0, theta0, psi0, ...
 phidot0, thetadot0, psidot0] = flatball_log_get_ic( tsc);

title_str = sprintf( '[%4s %5.2f %4s %5.2f  %4s %5.2f] (m) [%4s %5.2f  %4s %5.2f  %4s %5.2f] (m/s)\n[%4s %5.2f  %4s %5.2f  %4s %5.2f] (rad) [%4s %5.2f  %4s %5.2f  %4s %5.2f] (rad/s)\n', ...
    'x=', x0, 'y=', y0, 'z=', z0, ... 
    'u=', u0, 'v=', v0, 'w=', w0, ...
    '\phi=', phi0, '\theta=', theta0, '\psi=', psi0, ...
    '\phi\prime=', phidot0, '\theta\prime=', thetadot0, '\psi\prime=', psidot0 );

%%
new_formatted_fig( 'ahrs-v-batt');
tsc.batt_volt.plot;
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
ylabel( 'v');
ylim([3 4.5]);
grid on;
title( '');

snapnow;

%%
new_formatted_fig( 'ahrs-ambient');
yyaxis left;
tsc.altitude.plot;
ylabel( 'm');
ylim([tsc.altitude.mean-3*tsc.altitude.std tsc.altitude.mean+3*tsc.altitude.std]);
grid on;
yyaxis right;
tsc.temp.plot;
% hold off;
ylabel( 'degC');
ylim([10 40]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'acc-3d-scatter');
plot3( tsc.acc_x.Data, tsc.acc_y.Data, tsc.acc_z.Data, '.');
hold on;
plot3( tsc.acc_x.Data(idx_launch_end:idx_impact), tsc.acc_y.Data(idx_launch_end:idx_impact), tsc.acc_z.Data(idx_launch_end:idx_impact), '.');
hold off;
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'acc');
hold all;
tsc.acc_x.plot;
tsc.acc_y.plot;
tsc.acc_z.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'acc-x', 'acc-y', 'acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'acc');
hold all;
tsc.acc_x.plot;
tsc.acc_y.plot;
tsc.acc_z.plot;
% tsc.acc.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'acc-x', 'acc-y', 'acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'fe-acc');
hold all;
tsc.fe_acc_x.plot;
tsc.fe_acc_y.plot;
tsc.fe_acc_z.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'fe-acc-x', 'fe-acc-y', 'fe-acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'fe-acc');
hold all;
tsc.fe_acc_x.plot;
tsc.fe_acc_y.plot;
tsc.fe_acc_z.plot;
% tsc.fe_acc.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'fe-acc-x', 'fe-acc-y', 'fe-acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'fl-acc');
hold all;
tsc.fl_acc_x.plot;
tsc.fl_acc_y.plot;
tsc.fl_acc_z.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'fl-acc-x', 'fl-acc-y', 'fl-acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'fl-acc');
hold all;
tsc.fl_acc_x.plot;
tsc.fl_acc_y.plot;
tsc.fl_acc_z.plot;
% tsc.fl_acc.plot;
hold off;
ylabel( 'm/s^2');
ylim([-1 1] .* 160);
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'fl-acc-x', 'fl-acc-y', 'fl-acc-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'gyro-3d-scatter');
plot3( tsc.gyro_x.Data, tsc.gyro_y.Data, tsc.gyro_z.Data, '.');
hold on;
plot3( tsc.gyro_x.Data(idx_launch_end:idx_impact), tsc.gyro_y.Data(idx_launch_end:idx_impact), tsc.gyro_z.Data(idx_launch_end:idx_impact), '.');
hold off;
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'gyro');
yyaxis left;
hold all;
tsc.gyro_x.plot('-b');
tsc.gyro_y.plot('-r');
ylim([-1 1] .* 5000*C.DEG2RAD);
yyaxis right;
tsc.gyro_z.plot('Color','#EDB120');
ylim([-1 1] .* 5000*C.DEG2RAD);
hold off;
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'gyro-x', 'gyro-y', 'gyro-z'}, 'Location','best');
ylabel( 'rad/sec');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'gyro');
yyaxis left;
hold all;
tsc.gyro_x.plot('-b');
tsc.gyro_y.plot('-r');
ylim([-1 1] .* 5000*C.DEG2RAD);
yyaxis right;
tsc.gyro_z.plot('Color','#EDB120');
ylim([-1 1] .* 5000*C.DEG2RAD);
hold off;
ylabel( 'rad/sec');
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'gyro-x', 'gyro-y', 'gyro-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'mag-3d-scatter');
plot3( tsc.mag_x.Data, tsc.mag_y.Data, tsc.mag_z.Data, '.');
hold on;
plot3( tsc.mag_x.Data(idx_launch_end:idx_impact), tsc.mag_y.Data(idx_launch_end:idx_impact), tsc.mag_z.Data(idx_launch_end:idx_impact), '.');
hold off;
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'mag');
hold all;
tsc.mag_x.plot;
tsc.mag_y.plot;
tsc.mag_z.plot;
hold off;
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'mag-x', 'mag-y', 'mag-z'}, 'Location','best');
ylabel( 'uT');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'mag');
hold all;
tsc.mag_x.plot;
tsc.mag_y.plot;
tsc.mag_z.plot;
hold off;
ylabel( 'uT');
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'mag-x', 'mag-y', 'mag-z'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'ahrs-orientation');

ax1 = subplot(211);
hold all;
tsc.pitch.plot;
tsc.roll.plot;
hold off;
ylabel( 'deg');
ylim([-1 1] .* (200));
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'pitch', 'roll'}, 'Location','best');
grid on;

title( title_str);

ax2 = subplot(212);
hold all;
tsc.yaw.plot;
hold off;
ylabel( 'deg');
ylim([-1 1] .* (200));
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'yaw'}, 'Location','best');
grid on;

linkaxes( [ax1 ax2], 'x');

snapnow;

%%
new_formatted_fig( 'ahrs-orientation');

ax1 = subplot(211);
hold all;
tsc.pitch.plot;
tsc.roll.plot;
hold off;
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'pitch', 'roll'}, 'Location','best');
ylabel( 'deg');
grid on;

title( title_str);

ax2 = subplot(212);
hold all;
tsc.yaw.plot;
hold off;
ylabel( 'deg');
ylim([-1 1] .* (200));
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'yaw'}, 'Location','best');
grid on;

linkaxes( [ax1 ax2], 'x');

snapnow;

%%
new_formatted_fig( 'vel');
hold all;
tsc.u.plot;
tsc.v.plot;
tsc.w.plot;
hold off;
ylabel( 'm/s');
% ylim([-1 1] .* 160);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'u', 'v'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'vel');
hold all;
tsc.u.plot;
tsc.v.plot;
tsc.w.plot;
hold off;
ylabel( 'm/s');
% ylim([-1 1] .* 160);
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'u', 'v'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'pos');
hold all;
tsc.x.plot;
tsc.y.plot;
tsc.z.plot;
hold off;
ylabel( 'm');
% ylim([-1 1] .* 160);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'x', 'y'}, 'Location','best');
grid on;
title( title_str);

snapnow;

%%
new_formatted_fig( 'pos');
hold all;
tsc.x.plot;
tsc.y.plot;
tsc.z.plot;
hold off;
ylabel( 'm');
% ylim([-1 1] .* 160);
xlim([tsc.Time(idx_launch_beg)-0.25 tsc.Time(idx_impact)+0.25]);
add_vert_marker( tsc.Time(idx_launch_beg));
add_vert_marker( tsc.Time(idx_launch_end));
add_vert_marker( tsc.Time(idx_impact));
legend( {'x', 'y'}, 'Location','best');
grid on;
title( title_str);

snapnow;

