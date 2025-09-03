%% initialization
flatball_init_disc_params;

fldr_exp = '.\wrk\f-dev-3.1.3-datalogs\2024\07-oct';

%% thrower - mvs - right handed

%% back-hand
n_exp = 'x0000000b';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% back-hand
n_exp = 'x0000000c';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% fore-hand
n_exp = 'x0000000d';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
% close all;

%% fore-hand
n_exp = 'x0000000e';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% hammer
n_exp = 'x0000000f';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% hammer
n_exp = 'x00000010';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;
