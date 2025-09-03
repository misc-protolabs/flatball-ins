%% launch force and moments
new_formatted_fig( 'F,M-launch');

ax1 = subplot(211);
hold on;
tsc.Fx_launch.plot;
tsc.Fy_launch.plot
tsc.Fz_launch.plot;
hold off;
legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'F-launch');
ylabel( 'N');
xlim([0 1.3]);
ylim( [-150 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Mx_launch.plot;
tsc.My_launch.plot
tsc.Mz_launch.plot;
hold off;
legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'M-launch');
ylabel( 'N-m');
xlim([0 1.3]);
ylim( [-1 1] .* 10);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%%
new_formatted_fig( 'P,E-launch');

hold on;
tsc.pwr_trans.plot;
tsc.pwr_rot.plot;
hold off;
ylabel( 'W');
yyaxis right;
tsc.e_launch.plot;
ylabel( 'J');
legend( {'P-trans-launch', 'P-rot-launch', 'E-huck (J)'}, 'Location','best');
title( 'P,E-launch');
xlim([0 1.3]);
% ylim( [-1 1] .* 10);
xlabel( title_str);
grid on;

snapnow;

%% exit early (if called from a script)
return;

%% launch Fx
new_formatted_fig( 'Fx-launch');

ax1 = subplot(211);
hold on;
tsc.Fx_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'Fx-launch');
ylabel( 'N');
xlim([0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Fx_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'm/s');
xlim([0 1.3]);
% ylim( [-1 1] .* 10);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% launch Fy
new_formatted_fig( 'Fy-launch');

ax1 = subplot(211);
hold on;
tsc.Fy_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'Fy-launch');
ylabel( 'N');
xlim([0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Fy_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'm/s');
xlim([0 1.3]);
% ylim( [-1 1] .* 10);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% launch Fz
new_formatted_fig( 'Fz-launch');

ax1 = subplot(211);
hold on;
tsc.Fz_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'Fz-launch');
ylabel( 'N');
xlim([0.0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Fz_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'm');
xlim([0.0 1.3]);
% ylim( [-1 1] .* 10);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% launch Mx
new_formatted_fig( 'Mx-launch');

ax1 = subplot(211);
hold on;
tsc.Mx_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'M\phi-launch');
ylabel( 'N*m');
xlim([0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Mx_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'rad');
xlim([0 1.3]);
% ylim( [-1 1] .* 10);
% xlabel( title_str);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% launch My
new_formatted_fig( 'My-launch');

ax1 = subplot(211);
hold on;
tsc.My_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'M\theta-launch');
ylabel( 'N');
xlim([0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.My_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'rad');
xlim([0 1.3]);
% ylim( [-1 1] .* 10);
% xlabel( title_str);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% launch Mz
new_formatted_fig( 'Mz-launch');

ax1 = subplot(211);
hold on;
tsc.Mz_launch.plot;
hold off;
% legend( {'Fx-launch', 'Fy-launch', 'Fz-launch'}, 'Location','best');
title( 'M\psi-launch');
ylabel( 'N*m');
xlim([0.0 1.3]);
% ylim( [-10 150]);
grid on;

ax2 = subplot(212);
hold on;
tsc.Mz_launch_err.plot;
hold off;
% legend( {'Mx-launch', 'My-launch', 'Mz-launch'}, 'Location','best');
title( 'err');
ylabel( 'rad/s');
xlim([0.0 1.3]);
% ylim( [-1 1] .* 10);
% xlabel( title_str);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;
