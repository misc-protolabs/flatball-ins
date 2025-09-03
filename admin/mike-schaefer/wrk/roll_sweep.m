%% start from scratch
clc;
close all;
startup;

% initialize ambient and disc properties
flatball_6dof_init;
sys = 'CandP';
load_system( sys);

%% perform roll sweep
uL = 15;
phiL = linspace( -30*C.DEG2RAD, 30*C.DEG2RAD, 20);
thetaL = 15*C.DEG2RAD;
advr = 0.75;
rL = (2*uL*advr/disc.d);
rangex = zeros(length(phiL),1);
rangey = zeros(length(phiL),1);
rangez = zeros(length(phiL),1);
duration = zeros(length(phiL),1);
for idx = 1:length( phiL)
    Y0 = [0 0 -1, phiL(idx) thetaL 0, uL 0 0, 0 0 rL];
    simout = sim( sys);
    tsc = flatball_6dof_post( simout.logsout, disc, amb);
    rangex(idx) = tsc.x.Data(end);
    rangey(idx) = tsc.y.Data(end);
    rangez(idx) = tsc.z.Data(end);
    duration(idx) = tsc.Time(end);
    toc( tic0);
end

%%
new_formatted_fig( 'landing position');
plot( phiL .* C.RAD2DEG, rangey, '-*');
% plot( rangex, rangey, '*');
% xlim( [0 max( rangex)*2]);
grid on;
xlabel( 'm');
ylabel( 'phi-launc (deg)')
% ax1 = subplot( 211);
% hold on;
% plot( thetaL .* C.RAD2DEG, rangex, '-*');
% % plot( thetaL .* C.RAD2DEG, rangey, '-*');
% % plot( thetaL .* C.RAD2DEG, rangez, '-*');
% hold off;
% grid on;
% ylabel( 'x (m)');
% ax2 = subplot( 212);
% plot( thetaL .* C.RAD2DEG, duration, '-*');
% grid on;
% ylabel( 't (s)');
% xlabel( 'theta-launch (deg)')
% linkaxes( [ax1 ax2], 'x');
