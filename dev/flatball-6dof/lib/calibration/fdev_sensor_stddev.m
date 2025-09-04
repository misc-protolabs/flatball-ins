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
addpath ./media/calibration-data/

SN_str = "SN-2024110603";

% fname = 'x00000019.csv';
fname = sprintf( "f-dev-cal-rest-%s.csv", SN_str);
fname = which( fname);
tsc = flatball_log_proc( fname);

%%
fprintf( 'sensors at rest, mean and standard deviations\n');
fprintf( '%15s - %10s - %9s\n', 'sensor', 'mean', 'sigma');
fprintf( '%15s - %10.4f - %5.3e\n', 'acc-x', tsc.acc_x.mean, tsc.acc_x.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'acc-y', tsc.acc_y.mean, tsc.acc_y.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'acc-z', tsc.acc_z.mean, tsc.acc_z.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'gyro-x', tsc.gyro_x.mean, tsc.gyro_x.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'gyro-y', tsc.gyro_y.mean, tsc.gyro_y.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'gyro-z', tsc.gyro_z.mean, tsc.gyro_z.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'mag-x', tsc.mag_x.mean, tsc.mag_x.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'mag-y', tsc.mag_y.mean, tsc.mag_y.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'mag-z', tsc.mag_z.mean, tsc.mag_z.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'patm', tsc.patm.mean, tsc.patm.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'temp', tsc.temp.mean, tsc.temp.std);
fprintf( '%15s - %10.4f - %5.3e\n', 'altitude', tsc.altitude.mean, tsc.altitude.std);

%%
title_str = sprintf( "%s - sensor noise", SN_str);

%%
new_formatted_fig( 'fdev-dt');
tsc.dt.plot;
ylim([0.0 0.10]);
ylabel( 's');
title( title_str);

grid on;
snapnow;

%%
new_formatted_fig( 'batt-volt');
tsc.batt_volt.plot;
ylim([3.5 4.5]);
ylabel( 'v');
title( title_str);

grid on;
snapnow;

%%
new_formatted_fig( 'altitude');
hold on;
tsc.altitude.plot;
hold off;
ylabel( 'm');
ylim( [tsc.altitude.min-2 tsc.altitude.max+2]);
title( title_str);

grid on;
snapnow;

%%
new_formatted_fig( 'temperature');
hold on;
tsc.temp.plot;
hold off;
ylabel( 'degC');
ylim( [tsc.temp.min-2 tsc.temp.max+2]);
title( title_str);

grid on;
snapnow;
