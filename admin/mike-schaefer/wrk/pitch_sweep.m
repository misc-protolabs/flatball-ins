%% start from scratch
clc;
close all;
startup;

% initialize ambient and disc properties
flatball_6dof_init;
sys = 'flatball_6dof';
load_system( sys);

%% perform pitch sweep

uL = 19;
thetaL = linspace( -30*C.DEG2RAD, 85*C.DEG2RAD, (85+30+1));
advr = 0.65;
rL = (2*uL*advr/disc.d);

rangex = zeros(length(thetaL),1);
rangey = zeros(length(thetaL),1);
rangez = zeros(length(thetaL),1);
duration = zeros(length(thetaL),1);

% mat_fnames = cell(length(thetaL),1);

for idx = 1:length( thetaL)
    exp = datestr( now, 30);
    Y0 = [0 0 -1, 0 thetaL(idx) 0, uL 0 0, 0 0 rL];
    simout = sim( sys);
    tsc = flatball_6dof_post( simout.logsout, disc, amb);
    
    rangex(idx) = tsc.x.Data(end);
    rangey(idx) = tsc.y.Data(end);
    rangez(idx) = tsc.z.min * -1;
    duration(idx) = tsc.Time(end);
end
toc( tic0);

%% plot range and duration vs thetaL

new_formatted_fig( 'range, duration');
ax1 = subplot( 211);
hold on;
plot( thetaL .* C.RAD2DEG, rangex);
ylabel( 'm');
yyaxis right;
plot( thetaL .* C.RAD2DEG, rangey);
plot( thetaL .* C.RAD2DEG, rangez);
hold off;
grid on;
ylabel( 'm');
legend( {'x-final', 'y-final', 'z-up-max'}, 'Location','best');
title( sprintf( 'uL = %5.2f (m/s), rL = %5.2f (rad/s), advr = %5.2f', uL, rL, advr));
ax2 = subplot( 212);
plot( thetaL .* C.RAD2DEG, duration);
grid on;
ylabel( 's');
xlabel( '\thetaL (deg)');
legend( 'time-aloft', 'Location','best');
linkaxes( [ax1 ax2], 'x');
snapnow;
