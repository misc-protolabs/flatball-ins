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


%%
new_formatted_fig( 'imu-sys1');
subplot(211);
tsc.temp.plot;
ylim([10 40]);
ylabel( 'degC');
subplot(212);
tsc.batt_volt.plot;
ylim([3 4.5]);
ylabel( 'v');

grid on;
snapnow;

%%
new_formatted_fig( 'dt');
tsc.dt.plot;
ylim([0.0 0.10]);
ylabel( 's');

grid on;
snapnow;

%%
new_formatted_fig( 'acc');
hold all;
tsc.acc_x.plot;
tsc.acc_y.plot;
tsc.acc_z.plot;
hold off;
legend( {'acc-x', 'acc-y', 'acc-z'}, 'Location','best');
ylabel( 'm/s^2');
% ylim([-1 1] .* 16);

grid on;
snapnow;

%%
new_formatted_fig( 'gyro');
hold all;
tsc.gyro_x.plot;
tsc.gyro_y.plot;
tsc.gyro_z.plot;
hold off;
legend( {'gyro-x', 'gyro-y', 'gyro-z'}, 'Location','best');
ylabel( 'rad/sec');
% ylim([-1 1] .* (2*pi*10));

grid on;
snapnow;

%%
new_formatted_fig( 'mag');
hold all;
tsc.mag_x.plot;
tsc.mag_y.plot;
tsc.mag_z.plot;
hold off;
legend( {'mag-x', 'mag-y', 'mag-z'}, 'Location','best');
ylabel( 'rad/sec');
% ylim([-1 1] .* (2*pi*10));

grid on;
snapnow;

%%
new_formatted_fig( 'acc-3d-scatter');
plot3( tsc.acc_x.Data, tsc.acc_y.Data, tsc.acc_z.Data, '.');
hold all;
plot3( tsc.acc_x_cal.Data, tsc.acc_y_cal.Data, tsc.acc_z_cal.Data, '.');
hold off;

legend( {'uncalibrated', 'calibrated'}, 'Location','best');
title( title_str);

grid on;
snapnow;

%%
new_formatted_fig( 'gyro-3d-scatter');
plot3( tsc.gyro_x.Data, tsc.gyro_y.Data, tsc.gyro_z.Data, '.');
hold all;
plot3( tsc.gyro_x_cal.Data, tsc.gyro_y_cal.Data, tsc.gyro_z_cal.Data, '.');
hold off;

legend( {'uncalibrated', 'calibrated'}, 'Location','best');
title( title_str);

grid on;
snapnow;

%%
new_formatted_fig( 'mag-3d-scatter');
plot3( tsc.mag_x.Data, tsc.mag_y.Data, tsc.mag_z.Data, '.');
hold all;
plot3( tsc.mag_x_cal.Data, tsc.mag_y_cal.Data, tsc.mag_z_cal.Data, '.');
hold off;
% xlim([-1 1] .* 100); xlabel( 'uT');
% ylim([-1 1] .* 100); ylabel( 'uT');
% zlim([-1 1] .* 100); zlabel( 'uT');
legend( {'uncalibrated', 'calibrated'}, 'Location','best');
title( title_str);

grid on;
snapnow;

%%
grid_on_all;
