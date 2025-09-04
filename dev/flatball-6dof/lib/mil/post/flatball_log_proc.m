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

function tsc = flatball_log_proc( fname)
%%
%
% idx	tstamp	dt	batt-volt	imu-deg-c	acc-x-raw	acc-y-raw	acc-z-raw	gyro-x-raw	gyro-y-raw	gyro-z-raw	pitch	roll	yaw
% idx	tstamp	dt	batt-volt	imu-deg-c	acc-x-raw	acc-y-raw	acc-z-raw	gyro-x-raw	gyro-y-raw	gyro-z-raw	mag-x-raw	mag-y-raw	mag-z-raw	acc-x	acc-y	acc-z	gyro-x	gyro-y	gyro-z	mag-x	mag-y	mag-z
% idx	dt	batt-v	acc-x	acc-y	acc-z	gyro-x	gyro-y	gyro-z	mag-x	mag-y	mag-z	pitch	roll	yaw	fe-acc-x	fe-acc-y	fe-acc-z	fl-acc-x	fl-acc-y	fl-acc-z
% idx,dt,batt-v,altitude-m,amb-temp-degC,acc-x,acc-y,acc-z,gyro-x,gyro-y,gyro-z,mag-x,mag-y,mag-z,qw,qx,qy,qz,pitch,roll,yaw,fe-acc-x,fe-acc-y,fe-acc-z,fl-acc-x,fl-acc-y,fl-acc-z

csv_dat = readmatrix( fname);
tbl_dat = readtable( fname);
hdr_one = char( tbl_dat.Properties.VariableNames{1});
sz = size( csv_dat);

if( sz(2) == 17)
    ver = 1.2;
elseif( sz(2) == 26 && strcmp( hdr_one, 't'))
    ver = 3.1;
elseif( sz(2) == 27)
    ver = 3.2;
elseif( sz(2) == 26 && strcmp( hdr_one, 'idx'))
    ver = 3.3;
else
    ver = 3.3;
end

if( ver == 1.2)
    tsc = parse_v12( csv_dat);
elseif( ver == 3.1)
    tsc = parse_v31( csv_dat);
elseif( ver == 3.2)
    tsc = parse_v32( csv_dat);
elseif( ver == 3.3)
    tsc = parse_v33( csv_dat);
else
    tsc = parse_v33( csv_dat);
end

if( ver == 3.2)
%%
acc = tsc.acc_x .* tsc.acc_x + tsc.acc_y .* tsc.acc_y + tsc.acc_z .* tsc.acc_z;
acc.Data = sqrt( acc.Data);
acc.Name = 'acc';

acc_dot = ts_derrivative( acc);
acc_dot.Name = 'acc_dot';

fe_acc = tsc.fe_acc_x .* tsc.fe_acc_x + tsc.fe_acc_y .* tsc.fe_acc_y + tsc.fe_acc_z .* tsc.fe_acc_z;
fe_acc.Data = sqrt( fe_acc.Data);
fe_acc.Name = 'fe_acc';

fl_acc = tsc.fl_acc_x .* tsc.fl_acc_x + tsc.fl_acc_y .* tsc.fl_acc_y + tsc.fl_acc_z .* tsc.fl_acc_z;
fl_acc.Data = sqrt( fl_acc.Data);
fl_acc.Name = 'fl_acc';

tsc = tsc.addts( acc);
tsc = tsc.addts( fe_acc);
tsc = tsc.addts( fl_acc);
tsc = tsc.addts( acc_dot);
end
end

function tsc = parse_v12( csv_dat)
x0_ofst = 100;
xfinal_ofst = 10;

log_idx = csv_dat(x0_ofst:end-xfinal_ofst,1);
dt = csv_dat(x0_ofst:end-xfinal_ofst,2);

batt_volt = csv_dat(x0_ofst:end-xfinal_ofst,3);
altitude = csv_dat(x0_ofst:end-xfinal_ofst,4);
temp = csv_dat(x0_ofst:end-xfinal_ofst,5);

acc_x = csv_dat(x0_ofst:end-xfinal_ofst,6);
acc_y = csv_dat(x0_ofst:end-xfinal_ofst,7);
acc_z = csv_dat(x0_ofst:end-xfinal_ofst,8);

gyro_x = csv_dat(x0_ofst:end-xfinal_ofst,9);
gyro_y = csv_dat(x0_ofst:end-xfinal_ofst,10);
gyro_z = csv_dat(x0_ofst:end-xfinal_ofst,11);

mag_x = csv_dat(x0_ofst:end-xfinal_ofst,12);
mag_y = csv_dat(x0_ofst:end-xfinal_ofst,13);
mag_z = csv_dat(x0_ofst:end-xfinal_ofst,14);

roll = csv_dat(x0_ofst:end-xfinal_ofst,12);
pitch = csv_dat(x0_ofst:end-xfinal_ofst,13);
yaw = csv_dat(x0_ofst:end-xfinal_ofst,14);

% dt = dt .* 1e-6;
dt_dl9dof = 0.01;
t = log_idx * dt_dl9dof;

%%
tsc = tscollection;

ts = timeseries( dt, t, 'Name', 'dt');
tsc = tsc.addts( ts);

ts = timeseries( batt_volt, t, 'Name', 'batt_volt');
tsc = tsc.addts( ts);
ts = timeseries( altitude, t, 'Name', 'altitude');
tsc = tsc.addts( ts);
ts = timeseries( temp, t, 'Name', 'temp');
tsc = tsc.addts( ts);

ts = timeseries( acc_x, t, 'Name', 'acc_x');
tsc = tsc.addts( ts);
ts = timeseries( acc_y, t, 'Name', 'acc_y');
tsc = tsc.addts( ts);
ts = timeseries( acc_z, t, 'Name', 'acc_z');
tsc = tsc.addts( ts);

ts = timeseries( gyro_x, t, 'Name', 'gyro_x');
tsc = tsc.addts( ts);
ts = timeseries( gyro_y, t, 'Name', 'gyro_y');
tsc = tsc.addts( ts);
ts = timeseries( gyro_z, t, 'Name', 'gyro_z');
tsc = tsc.addts( ts);

ts = timeseries( mag_x, t, 'Name', 'mag_x');
tsc = tsc.addts( ts);
ts = timeseries( mag_y, t, 'Name', 'mag_y');
tsc = tsc.addts( ts);
ts = timeseries( mag_z, t, 'Name', 'mag_z');
tsc = tsc.addts( ts);

ts = timeseries( roll, t, 'Name', 'roll');
tsc = tsc.addts( ts);
ts = timeseries( pitch, t, 'Name', 'pitch');
tsc = tsc.addts( ts);
ts = timeseries( yaw, t, 'Name', 'yaw');
tsc = tsc.addts( ts);

end

function tsc = parse_v31( csv_dat)
x0_ofst = 100;
xfinal_ofst = 10;

log_idx = csv_dat(x0_ofst:end-xfinal_ofst,1);
dt = csv_dat(x0_ofst:end-xfinal_ofst,3);
batt_volt = csv_dat(x0_ofst:end-xfinal_ofst,4);
temp = csv_dat(x0_ofst:end-xfinal_ofst,5);

acc_x = csv_dat(x0_ofst:end-xfinal_ofst,6);
acc_y = csv_dat(x0_ofst:end-xfinal_ofst,7);
acc_z = csv_dat(x0_ofst:end-xfinal_ofst,8);

gyro_x = csv_dat(x0_ofst:end-xfinal_ofst,9);
gyro_y = csv_dat(x0_ofst:end-xfinal_ofst,10);
gyro_z = csv_dat(x0_ofst:end-xfinal_ofst,11);

mag_x = csv_dat(x0_ofst:end-xfinal_ofst,12);
mag_y = csv_dat(x0_ofst:end-xfinal_ofst,13);
mag_z = csv_dat(x0_ofst:end-xfinal_ofst,14);

dt = dt .* 1e-6;
dt_dl9dof = 0.01;
t = log_idx * dt_dl9dof;

%%
tsc = tscollection;

ts = timeseries( dt, t, 'Name', 'dt');
tsc = tsc.addts( ts);

ts = timeseries( batt_volt, t, 'Name', 'batt_volt');
tsc = tsc.addts( ts);
ts = timeseries( temp, t, 'Name', 'temp');
tsc = tsc.addts( ts);

g_mag = 1; % 9.80; % m/s^2 --> FIXME: remove if unnecessary!..
ts = timeseries( acc_x .* g_mag, t, 'Name', 'acc_x');
tsc = tsc.addts( ts);
ts = timeseries( acc_y .* g_mag, t, 'Name', 'acc_y');
tsc = tsc.addts( ts);
ts = timeseries( acc_z .* g_mag, t, 'Name', 'acc_z');
tsc = tsc.addts( ts);

ts = timeseries( gyro_x, t, 'Name', 'gyro_x');
tsc = tsc.addts( ts);
ts = timeseries( gyro_y, t, 'Name', 'gyro_y');
tsc = tsc.addts( ts);
ts = timeseries( gyro_z, t, 'Name', 'gyro_z');
tsc = tsc.addts( ts);

ts = timeseries( mag_x, t, 'Name', 'mag_x');
tsc = tsc.addts( ts);
ts = timeseries( mag_y, t, 'Name', 'mag_y');
tsc = tsc.addts( ts);
ts = timeseries( mag_z, t, 'Name', 'mag_z');
tsc = tsc.addts( ts);

end

function tsc = parse_v32( csv_dat)
disp( 'Parsing v32 csv...');

x0_ofst = find( csv_dat(1:end,1) == 0, 1);
if( isempty( x0_ofst))
    x0_ofst = find( csv_dat(1:end,1) == 1, 1);
end
xfinal_ofst = 0;

log_idx = csv_dat(x0_ofst:end-xfinal_ofst,1);
dt = csv_dat(x0_ofst:end-xfinal_ofst,2);
batt_volt = csv_dat(x0_ofst:end-xfinal_ofst,3);
patm = csv_dat(x0_ofst:end-xfinal_ofst,4);
temp = csv_dat(x0_ofst:end-xfinal_ofst,5);
acc_x = csv_dat(x0_ofst:end-xfinal_ofst,6);
acc_y = csv_dat(x0_ofst:end-xfinal_ofst,7);
acc_z = csv_dat(x0_ofst:end-xfinal_ofst,8);
gyro_x = csv_dat(x0_ofst:end-xfinal_ofst,9);
gyro_y = csv_dat(x0_ofst:end-xfinal_ofst,10);
gyro_z = csv_dat(x0_ofst:end-xfinal_ofst,11);
mag_x = csv_dat(x0_ofst:end-xfinal_ofst,12);
mag_y = csv_dat(x0_ofst:end-xfinal_ofst,13);
mag_z = csv_dat(x0_ofst:end-xfinal_ofst,14);
roll = csv_dat(x0_ofst:end-xfinal_ofst,15);
pitch = csv_dat(x0_ofst:end-xfinal_ofst,16);
yaw = csv_dat(x0_ofst:end-xfinal_ofst,17);
qw = csv_dat(x0_ofst:end-xfinal_ofst,18);
qx = csv_dat(x0_ofst:end-xfinal_ofst,19);
qy = csv_dat(x0_ofst:end-xfinal_ofst,20);
qz = csv_dat(x0_ofst:end-xfinal_ofst,21);
fe_acc_x = csv_dat(x0_ofst:end-xfinal_ofst,22);
fe_acc_y = csv_dat(x0_ofst:end-xfinal_ofst,23);
fe_acc_z = csv_dat(x0_ofst:end-xfinal_ofst,24);
fl_acc_x = csv_dat(x0_ofst:end-xfinal_ofst,25);
fl_acc_y = csv_dat(x0_ofst:end-xfinal_ofst,26);
fl_acc_z = csv_dat(x0_ofst:end-xfinal_ofst,27);

%t = log_idx .* 0.01;
% t = [0; cumsum( dt(x0_ofst:end-xfinal_ofst))]';
% dt = dt + 0.01;
t = cumsum( dt) - mean(dt);

%%
tsc = tscollection;

ts = timeseries( dt, t, 'Name', 'dt');
tsc = tsc.addts( ts);
ts = timeseries( log_idx, t, 'Name', 'idx');
tsc = tsc.addts( ts);

ts = timeseries( batt_volt, t, 'Name', 'batt_volt');
tsc = tsc.addts( ts);

g_mag = 1; % 9.80; % m/s^2 --> FIXME: remove if unnecessary!..
ts = timeseries( acc_x .* g_mag, t, 'Name', 'acc_x');
tsc = tsc.addts( ts);
ts = timeseries( acc_y .* g_mag, t, 'Name', 'acc_y');
tsc = tsc.addts( ts);
ts = timeseries( acc_z .* g_mag, t, 'Name', 'acc_z');
tsc = tsc.addts( ts);

ts = timeseries( gyro_x, t, 'Name', 'gyro_x');
tsc = tsc.addts( ts);
ts = timeseries( gyro_y, t, 'Name', 'gyro_y');
tsc = tsc.addts( ts);
ts = timeseries( gyro_z, t, 'Name', 'gyro_z');
tsc = tsc.addts( ts);

ts = timeseries( mag_x, t, 'Name', 'mag_x');
tsc = tsc.addts( ts);
ts = timeseries( mag_y, t, 'Name', 'mag_y');
tsc = tsc.addts( ts);
ts = timeseries( mag_z, t, 'Name', 'mag_z');
tsc = tsc.addts( ts);

ts = timeseries( roll, t, 'Name', 'roll');
tsc = tsc.addts( ts);
ts = timeseries( pitch, t, 'Name', 'pitch');
tsc = tsc.addts( ts);
ts = timeseries( yaw, t, 'Name', 'yaw');
tsc = tsc.addts( ts);

ts = timeseries( fe_acc_x, t, 'Name', 'fe_acc_x');
tsc = tsc.addts( ts);
ts = timeseries( fe_acc_y, t, 'Name', 'fe_acc_y');
tsc = tsc.addts( ts);
ts = timeseries( fe_acc_z, t, 'Name', 'fe_acc_z');
tsc = tsc.addts( ts);

ts = timeseries( fl_acc_x, t, 'Name', 'fl_acc_x');
tsc = tsc.addts( ts);
ts = timeseries( fl_acc_y, t, 'Name', 'fl_acc_y');
tsc = tsc.addts( ts);
ts = timeseries( fl_acc_z, t, 'Name', 'fl_acc_z');
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'patm');
tsc = tsc.addts( ts);

ts = timeseries( temp, t, 'Name', 'temp');
tsc = tsc.addts( ts);

ts = timeseries( qw, t, 'Name', 'qw');
tsc = tsc.addts( ts);
ts = timeseries( qx, t, 'Name', 'qx');
tsc = tsc.addts( ts);
ts = timeseries( qy, t, 'Name', 'qy');
tsc = tsc.addts( ts);
ts = timeseries( qz, t, 'Name', 'qz');
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'altitude');
ts.Data = 44330.0 .* (1.0 - ((ts.Data ./ 100.0) ./ 1013.25) .^ 0.1903);
tsc = tsc.addts( ts);

end

function tsc = parse_v33( csv_dat)
disp( 'Parsing v33 csv...');

x0_ofst = find( csv_dat(1:end,1) == 0, 1);
if( isempty( x0_ofst))
    x0_ofst = find( csv_dat(1:end,1) == 1, 1);
end
xfinal_ofst = 0;

log_idx = csv_dat(x0_ofst:end-xfinal_ofst,1);
batt_volt = csv_dat(x0_ofst:end-xfinal_ofst,2);
patm = csv_dat(x0_ofst:end-xfinal_ofst,3);
temp = csv_dat(x0_ofst:end-xfinal_ofst,4);
acc_x = csv_dat(x0_ofst:end-xfinal_ofst,5);
acc_y = csv_dat(x0_ofst:end-xfinal_ofst,6);
acc_z = csv_dat(x0_ofst:end-xfinal_ofst,7);
gyro_x = csv_dat(x0_ofst:end-xfinal_ofst,8);
gyro_y = csv_dat(x0_ofst:end-xfinal_ofst,9);
gyro_z = csv_dat(x0_ofst:end-xfinal_ofst,10);
mag_x = csv_dat(x0_ofst:end-xfinal_ofst,11);
mag_y = csv_dat(x0_ofst:end-xfinal_ofst,12);
mag_z = csv_dat(x0_ofst:end-xfinal_ofst,13);
roll = csv_dat(x0_ofst:end-xfinal_ofst,14);
pitch = csv_dat(x0_ofst:end-xfinal_ofst,15);
yaw = csv_dat(x0_ofst:end-xfinal_ofst,16);
qw = csv_dat(x0_ofst:end-xfinal_ofst,17);
qx = csv_dat(x0_ofst:end-xfinal_ofst,18);
qy = csv_dat(x0_ofst:end-xfinal_ofst,19);
qz = csv_dat(x0_ofst:end-xfinal_ofst,20);
fe_acc_x = csv_dat(x0_ofst:end-xfinal_ofst,21);
fe_acc_y = csv_dat(x0_ofst:end-xfinal_ofst,22);
fe_acc_z = csv_dat(x0_ofst:end-xfinal_ofst,23);
fl_acc_x = csv_dat(x0_ofst:end-xfinal_ofst,24);
fl_acc_y = csv_dat(x0_ofst:end-xfinal_ofst,25);
fl_acc_z = csv_dat(x0_ofst:end-xfinal_ofst,26);

t = log_idx .* 0.01;
dt = [0; diff( t)];
% t = [0; cumsum( dt(x0_ofst:end-xfinal_ofst))]';
% dt = dt + 0.01;
% t = cumsum( dt) - mean(dt);

%%
tsc = tscollection;

C = evalin( 'base', 'C');

ts = timeseries( dt, t, 'Name', 'dt');
tsc = tsc.addts( ts);
ts = timeseries( log_idx, t, 'Name', 'idx');
tsc = tsc.addts( ts);

ts = timeseries( batt_volt, t, 'Name', 'batt_volt');
tsc = tsc.addts( ts);

g_mag = 9.81; % m/s^2 --> FIXME: remove if unnecessary!..
ts = timeseries( acc_x .* g_mag, t, 'Name', 'acc_x');
tsc = tsc.addts( ts);
ts = timeseries( acc_y .* g_mag, t, 'Name', 'acc_y');
tsc = tsc.addts( ts);
ts = timeseries( acc_z .* g_mag, t, 'Name', 'acc_z');
tsc = tsc.addts( ts);

ts = timeseries( gyro_x .* C.DEG2RAD, t, 'Name', 'gyro_x');
tsc = tsc.addts( ts);
ts = timeseries( gyro_y .* C.DEG2RAD, t, 'Name', 'gyro_y');
tsc = tsc.addts( ts);
ts = timeseries( gyro_z .* C.DEG2RAD, t, 'Name', 'gyro_z');
tsc = tsc.addts( ts);

ts = timeseries( mag_x, t, 'Name', 'mag_x');
tsc = tsc.addts( ts);
ts = timeseries( mag_y, t, 'Name', 'mag_y');
tsc = tsc.addts( ts);
ts = timeseries( mag_z, t, 'Name', 'mag_z');
tsc = tsc.addts( ts);

ts = timeseries( roll, t, 'Name', 'roll');
tsc = tsc.addts( ts);
ts = timeseries( pitch, t, 'Name', 'pitch');
tsc = tsc.addts( ts);
ts = timeseries( yaw, t, 'Name', 'yaw');
tsc = tsc.addts( ts);

ts = timeseries( fe_acc_x .* g_mag, t, 'Name', 'fe_acc_x');
tsc = tsc.addts( ts);
ts = timeseries( fe_acc_y .* g_mag, t, 'Name', 'fe_acc_y');
tsc = tsc.addts( ts);
ts = timeseries( fe_acc_z .* g_mag, t, 'Name', 'fe_acc_z');
tsc = tsc.addts( ts);

ts = timeseries( fl_acc_x .* g_mag, t, 'Name', 'fl_acc_x');
tsc = tsc.addts( ts);
ts = timeseries( fl_acc_y .* g_mag, t, 'Name', 'fl_acc_y');
tsc = tsc.addts( ts);
ts = timeseries( fl_acc_z .* g_mag, t, 'Name', 'fl_acc_z');
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'patm');
tsc = tsc.addts( ts);

ts = timeseries( temp, t, 'Name', 'temp');
tsc = tsc.addts( ts);

ts = timeseries( qw, t, 'Name', 'qw');
tsc = tsc.addts( ts);
ts = timeseries( qx, t, 'Name', 'qx');
tsc = tsc.addts( ts);
ts = timeseries( qy, t, 'Name', 'qy');
tsc = tsc.addts( ts);
ts = timeseries( qz, t, 'Name', 'qz');
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'altitude');
ts.Data = 44330.0 .* (1.0 - ((ts.Data ./ 100.0) ./ 1013.25) .^ 0.1903);
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'acc');
ts.Data = (tsc.acc_x.Data.^2 + tsc.acc_y.Data.^2 + tsc.acc_z.Data.^2).^0.5;
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'fe_acc');
ts.Data = (tsc.fe_acc_x.Data.^2 + tsc.fe_acc_y.Data.^2 + tsc.fe_acc_z.Data.^2).^0.5;
tsc = tsc.addts( ts);

ts = timeseries( patm, t, 'Name', 'fl_acc');
ts.Data = (tsc.fl_acc_x.Data.^2 + tsc.fl_acc_y.Data.^2 + tsc.fl_acc_z.Data.^2).^0.5;
tsc = tsc.addts( ts);

%% pose calcs
disc = evalin( 'base', 'disc');
[idx_launch_beg, idx_launch_end, idx_impact, ccw] = flatball_log_find_flight_window( tsc);

w = [tsc.gyro_x; tsc.gyro_y; tsc.gyro_z];
r = [disc.acc_r_x; disc.acc_r_y; 0];
acc_centr = cross( w, cross( w,r));

acc_x_flight = tsc.acc_x - acc_centr(1);
acc_y_flight = tsc.acc_y - acc_centr(2);
acc_z_flight = tsc.acc_z - acc_centr(3) + 9.81;

u_launch = ts_integrate_win( tsc.fl_acc_x .* -1, idx_launch_beg, idx_launch_end);
v_launch = ts_integrate_win( tsc.fl_acc_y .* -1, idx_launch_beg, idx_launch_end);
w_launch = ts_integrate_win( tsc.fl_acc_z .* 1, idx_launch_beg, idx_launch_end);

u = u_launch + ts_integrate_win( acc_x_flight, idx_launch_end, idx_impact);
v = v_launch + ts_integrate_win( acc_y_flight, idx_launch_end, idx_impact);
w = w_launch + ts_integrate_win( acc_z_flight, idx_launch_end, idx_impact);

x = ts_integrate_win( u, idx_launch_beg, idx_impact);
y = ts_integrate_win( v, idx_launch_beg, idx_impact);
z = ts_integrate_win( w, idx_launch_beg, idx_impact);

tsc = tsc.addts( u, 'u');
tsc = tsc.addts( v, 'v');
tsc = tsc.addts( w, 'w');
tsc = tsc.addts( x, 'x');
tsc = tsc.addts( y, 'y');
tsc = tsc.addts( z, 'z');

end
