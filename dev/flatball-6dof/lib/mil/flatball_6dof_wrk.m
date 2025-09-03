%% load model
tic0 = tic;

% sys = 'flatball_launch_IC';
sys = 'flatball_6dof';
open_system( sys);
flatball_6dof_init;
flatball_6dof_hucker_init;
toc( tic0);

%% X0 z == 1m above ground, at rest

% state vector == [x y z, phi theta psi, u v w, p q r]

X0_z1m_at_rest = [1e-3 1e-3 -1, -1e-6 1e-6 -1e-6, 0 0 1e-6, 0 0 -1e-6];

% XL_desired = [0 0 -45*C.IN2M, 5*C.DEG2RAD 15*C.DEG2RAD 0, 12 -9 0, 0 0 300*C.RPM2RADSEC];
XL_desired = [0 0 -45*C.IN2M, 5*C.DEG2RAD 5*C.DEG2RAD 0, 15 -3 0, 0 0 500*C.RPM2RADSEC];

%% run simulation
simout = sim( sys);
tsc = flatball_6dof_post( simout.logsout, disc, amb);
flatball_6dof_flight_metrics;
flatball_6dof_launch_plots;
flatball_6dof_plots;
toc( tic0);

%% legacy

% %% X0 target(s)
% % XL_desired = [-0.90 -0.63 -0.91 13.42 -0.41 0.001 -0.07 0.21 5.03 -14.94 -1.48 54.25];
% X0_hucker_001 = [0 0 -1, 13.5 -0.5 0, -4*pi/180 12*pi/180 0, 0 0 55];
% X0_hucker_002 = [0 0 -1, 13.5 0.5 0, -4*pi/180 12*pi/180 0, 0 0 55];
% X0_hucker_003 = [0 0 -1, 13.0 -1.5 0, -4*pi/180 12*pi/180 0, 0 0 55];
% X0_hucker_004 = [0 0 -1, 13.0 1.5 0, -4*pi/180 12*pi/180 0, 0 0 55];
% X0_hucker_005 = [0 0 -1, 18.0 -3.5 0, -4*pi/180 12.5*pi/180 0, 0 0 65];
% % X0_hucker_006 = [0 0 -1, 18.0 3.5 0, 4*pi/180 -12.5*pi/180 0, 0 0 -65];
% % crowther coefficients --> hucker --> ~35m, ~4s
% X0_hucker_006 = [0 0 -1, 18.0 3.5 0, 4*pi/180 -12.5*pi/180 0, 0 0 -65];
% 
% %% set the target to hucker-001 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_001;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% close all;
% 
% %% set the target to hucker-002 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_002;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% close all;
% 
% %% set the target to hucker-003 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_003;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% close all;
% 
% %% set the target to hucker-004 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_004;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% close all;
% 
% %% set the target to hucker-005 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_005;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% close all;
% 
% %% set the target to hucker-006 initial state, and run simulation
% flatball_6dof_init;
% flatball_6dof_hucker_init;
% XL_desired = X0_hucker_006;
% simout = sim( sys);
% tsc = flatball_6dof_post( logsout, disc, amb);
% flatball_6dof_flight_metrics;
% flatball_6dof_launch_plots;
% flatball_6dof_plots;
% toc( tic0);
% % close all;
