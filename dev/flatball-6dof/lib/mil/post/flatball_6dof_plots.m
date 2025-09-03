%%
xlim_t0 = 0.5;

%% energy dissipation

new_formatted_fig( 'energy-dissipation');

hold on;
tsc.ke_trans.plot;
tsc.ke_rot.plot;
tsc.ke_tot.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.');
hold off;
legend( {'E-trans', 'E-rot', 'E-tot'}, 'Location','best');
title( 'Energy Dissipation');
ylabel( 'J');
ylim( [0 1] .* 50);
xlim([xlim_t0 tsc.Time(end)]);
xlabel( title_str);
grid on;

snapnow;

%% F,M
new_formatted_fig( 'F,M');

ax1 = subplot(211);
hold on;
tsc.F2.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.');
hold off;
legend( {'Fx', 'Fy', 'Fz'}, 'Location','best');
title( 'F');
ylabel( 'N');
ylim( [-1 1] .* pi);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

ax2 = subplot(212);
hold on;
tsc.M2.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.');
hold off;
legend( {'M\phi', 'M\theta', 'M\psi'}, 'Location','best');
title( 'M');
ylabel( 'N-m');
ylim( [-1 1] .* 0.5);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% angle of attack, lift, drag
new_formatted_fig( 'alpha,lift,drag');

ax1 = subplot(211);
hold on;
tsc.alpha.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.');
hold off;
title( 'angle-of-attack');
ylabel( 'rad');
ylim( ax1, [-pi/10 pi/2]);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

ax2 = subplot(212);
hold on;
tsc.lift.plot;
tsc.drag.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.');
hold off;
legend( {'lift', 'drag'}, 'Location','best');
title( 'lift,drag');
ylabel( 'N');
ylim( ax2, [0 1] .* 10);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% p,q,r
new_formatted_fig( 'p,q,r');

ax1 = subplot(211);
hold on;
tsc.p.plot;
tsc.q.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
ylim( ax1, [-1 1] .* pi/2);
xlim([xlim_t0 tsc.Time(end)]);
legend( {'p', 'q'}, 'Location','best');
title( 'p,q');
ylabel( 'rad/sec');
grid on;

ax2 = subplot(212);
hold on;
tsc.r.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
legend( {'r'}, 'Location','best');
title( 'r');
ylabel( 'rad/sec');
ylim( [-1 1] .* 2*pi*15);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% v-mag
new_formatted_fig( 'v-mag');
hold on;
tsc.v_inf.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
title( '|v|');
ylabel( 'm/s');
ylim( [0 25]);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
snapnow;

%% Cl,Cd,AdvR
new_formatted_fig( 'Cl,Cd,AdvR');
hold on;
tsc.Clift.plot;
tsc.Cdrag.plot;
tsc.AdvR.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
legend( {'Cl', 'Cd', 'AdvR'}, 'Location','best');
title( 'Cd, Cl, AdvR');
ylabel( '');
ylim( [0 10]);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
snapnow;

%% position
new_formatted_fig( 'x,y,z');
ax1 = subplot(211);
hold on;
tsc.x.plot;
tsc.y.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
title( 'x,y');
ylabel( 'm');
legend( {'x', 'y'}, 'Location','best');
% xlabel( title_str);
ylim( [-1 1] .* 35);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

ax2 = subplot(212);
hold on;
tsc.z_up.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
title( 'z');
ylabel( 'm');
legend( {'z'}, 'Location','best');
ylim( [0 7.5]);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
linkaxes( [ax1 ax2], 'x');
snapnow;

%% veloctiy
new_formatted_fig( 'u,v,w');
hold on;
tsc.u.plot;
tsc.v.plot;
tsc.w.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
title( 'velocity');
ylabel( 'm/s');
legend( {'u', 'v', 'w'}, 'Location','best');
ylim( [-1 1] .* 25);
xlim([xlim_t0 tsc.Time(end)]);
grid on;

xlabel( title_str);
snapnow;

%% angles
new_formatted_fig( 'phi,theta');
hold on;
tsc.phi.plot;
tsc.theta.plot;
plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
hold off;
title( 'angles');
ylabel( 'rad');
ylim( [-1 1] .* pi/2);
xlim([xlim_t0 tsc.Time(end)]);
grid on;
legend( {'\phi', '\theta'}, 'Location','best');

xlabel( title_str);
snapnow;

%% angle-rates
% new_formatted_fig( 'angle-rates');
% 
% ax1 = subplot(211);
% hold on;
% tsc.p.plot;
% tsc.q.plot;
% plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
% hold off;
% title( 'angle-rates');
% ylabel( 'rad/s');
% legend( {'\phi\prime', '\theta\prime'}, 'Location','best');
% ylim( [-1 1] .* pi*5);
% xlim([xlim_t0 tsc.Time(end)]);
% grid on;
% 
% ax2 = subplot(212);
% hold on;
% tsc.r.plot;
% plot( [1 1] .* 0.60, [-1 1] .* 100, '-.')
% hold off;
% title( 'angle-rates');
% ylabel( 'rad/s');
% legend( {'\psi\prime'}, 'Location','best');
% ylim( [-1 1] .* 2*pi*15);
% xlim([xlim_t0 tsc.Time(end)]);
% grid on;
% 
% xlabel( title_str);
% linkaxes( [ax1 ax2], 'x');
% snapnow;
