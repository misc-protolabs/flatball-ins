%%
addpath ./arxiv/data/SN-2024110603/calibration/

SN_str = "SN-2024110603";

% fname = 'x00000019.csv';
fname = sprintf( "f-dev-cal-%s.csv", SN_str);
fname = which( fname);
% fname = 'x00000003.csv';
tsc = flatball_log_proc( fname);

%%
fdev_acc_cal;

%%
fdev_gyro_cal;

%%
fdev_mag_cal;

%%
title_str = sprintf( "%s - calibration", SN_str);
fdev_cal_plots;
