%% f-dev 3dof magnentometer calibration
%
% unit mounted to regulation ultimate frisbee and tested in open
% area with little to no known contribution from hard/soft iron
% inputs from external sources
% data and results below are for values copied from the gui manually
% per below
%

%% calibrate magnetometer (use as necessary)
% D = [tsc.mag_x.Data, tsc.mag_y.Data, tsc.mag_z.Data];
D = [tsc.mag_y.Data, tsc.mag_x.Data, tsc.mag_z.Data];   % NED
[A,b,expmfs] = magcal( D);
C = (D-b)*A; % calibrated data

%% add mag cal timeseries to tsc
t = tsc.Time;
ts = timeseries( C(:,1), t, 'Name', 'mag_x_cal');
tsc = tsc.addts( ts);
ts = timeseries( C(:,2), t, 'Name', 'mag_y_cal');
tsc = tsc.addts( ts);
ts = timeseries( C(:,3), t, 'Name', 'mag_z_cal');
tsc = tsc.addts( ts);

%%
fprintf( "%s - mag cal\n", SN_str);
A
b
expmfs
