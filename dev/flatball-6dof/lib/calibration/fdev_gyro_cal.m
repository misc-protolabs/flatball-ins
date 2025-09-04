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

%% f-dev 3dof gyroscope calibration
%
% unit mounted to regulation ultimate frisbee and tested in open
% area with little to no known contribution from hard/soft iron
% inputs from external sources
% data and results below are for values copied from the gui manually
% per below
%

%% calibrate gyroscope (use as necessary)
% D = [tsc.gyro_x.Data, tsc.gyro_y.Data, tsc.gyro_z.Data];
D = [tsc.gyro_y.Data, tsc.gyro_x.Data, -tsc.gyro_z.Data];   % NED
[A,b,expmfs] = magcal( D);
C = (D-b)*A; % calibrated data

%% add mag cal timeseries to tsc
t = tsc.Time;
ts = timeseries( C(:,1), t, 'Name', 'gyro_x_cal');
tsc = tsc.addts( ts);
ts = timeseries( C(:,2), t, 'Name', 'gyro_y_cal');
tsc = tsc.addts( ts);
ts = timeseries( C(:,3), t, 'Name', 'gyro_z_cal');
tsc = tsc.addts( ts);

%%
fprintf( "%s - gyro cal\n", SN_str);
A
b
expmfs
