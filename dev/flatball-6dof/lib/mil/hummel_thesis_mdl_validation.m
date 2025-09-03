%% reset tic0 and time things
tic0 = tic;

%% initialize ambient, disc (aka flatball), and control parameters for huck-er
flatball_6dof_init;
flatball_6dof_hucker_init;
toc( tic0);

%% f2302 X0 target(s)
X0_z1m_at_rest = [1e-3 1e-3 -1, -1e-6 1e-6 -1e-6, 0 0 1e-6, 0 0 -1e-6];
X0_hummel_f2302 = [ ...
    -0.90 -0.63 -0.91,  13.42 -0.41 0.001, ...
    -0.07 0.21 5.03,    -14.94 -1.48 54.25];
X0_crowther_f2302 = [ ...
    -0.90 -0.63 -0.91,  4.48 12.52 1.84, ...
    -0.07 0.21 5.03,    -26.25 -5.19 52.85];

%% load the flatball_6dof_IC model
%
% NOTE: this model runs from X0_desired, set below
%

sys = 'flatball_hummel_mdl_validation_IC';
open_system( sys);
toc( tic0);

%% set the target to z==1m (above ground), at rest, and run simulation
X0_desired = X0_z1m_at_rest;
simout = sim( sys);
tsc = flatball_6dof_post( logsout, disc, amb);
flatball_6dof_flight_metrics;
toc( tic0);
close all;

%% set the target to hummel-f2302 initial state, and run simulation
% X0_hummel_f2302 = [ ...
%     -0.90 -0.63 -0.91,  13.42 -0.41 0.001, ...
%     -0.07 0.21 5.03,    -14.94 -1.48 54.25];

X0_desired = X0_hummel_f2302;
simout = sim( sys);
tsc = flatball_6dof_post( logsout, disc, amb);
flatball_6dof_flight_metrics;
flatball_6dof_plots;
toc( tic0);
close all;

%% set the target to crowther-f2302 initial state, and run simulation
X0_desired = X0_crowther_f2302;
simout = sim( sys);
tsc = flatball_6dof_post( logsout, disc, amb);
flatball_6dof_flight_metrics;
flatball_6dof_plots;
toc( tic0);
close all;

% %% set the target to ??? initial state, and run simulation
% X0_desired = [0 0 -1, 2 0 0, 0 0 0, 0 0 30];
% simout = sim( sys);
% flatball_6dof_post;
% flatball_6dof_plots;
% toc( tic0);
% % close all;
% 
% %% set the target to ??? initial state, and run simulation
% X0_desired = [0 0 -1, 5 0 0, 0 0 0, 0 0 30];
% simout = sim( sys);
% flatball_6dof_post;
% flatball_6dof_plots;
% toc( tic0);
% % close all;
% 
% %% set the target to ??? initial state, and run simulation
% X0_desired = [0 0 -1, 0 5 0, 0 0 0, 0 0 30];
% simout = sim( sys);
% flatball_6dof_post;
% flatball_6dof_plots;
% toc( tic0);
% % close all;
