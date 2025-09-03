%% initial conditions for all states
if( tsc.Time(end) < k_launch_M_t_release)
    idx_release = 1;
else
    idx_release = max( k_launch_F_t_release, k_launch_M_t_release)*1000;
end

t_flight = tsc.Time(end) - 1.25;
x0 = tsc.x.Data(idx_release);
y0 = tsc.y.Data(idx_release);
z0 = tsc.z.Data(idx_release);
u0 = tsc.u.Data(idx_release);
v0 = tsc.v.Data(idx_release);
w0 = tsc.w.Data(idx_release);
phi0 = tsc.phi.Data(idx_release);
theta0 = tsc.theta.Data(idx_release);
psi0 = tsc.psi.Data(idx_release); % .* 0; % set to zero for display
p0 = tsc.p.Data(idx_release);
q0 = tsc.q.Data(idx_release);
r0 = tsc.r.Data(idx_release);

alpha0 = tsc.alpha.Data(idx_release);

X0_huck = [x0 y0 z0, u0 v0 w0, phi0 theta0 psi0 p0 q0 r0];
%% flight metrics

% fprintf( '%20s : %5.2f\n', '', 0);
% fprintf( '\n');
fprintf( '%20s : %5.2f\n', 'flight duration (s)', tsc.Time(end) - k_launch_Fx_t_launch_begin);
fprintf( '%20s : %5.2f\n', 'flight length (m)', tsc.flight_length.max);
fprintf( '%20s : %5.2f\n', '# of revs (rev)', tsc.revs.Data(end) .* (2*pi)^-1);
fprintf( '%20s : %5.2f\n', 'v-mag-peak (m/s)', tsc.v_inf.max);
% fprintf( '%20s : %5.2f\n', 'r-peak (rad/s)', tsc.r.max);
fprintf( '\n');
% fprintf( '%20s : [%5.2f %5.2f %5.2f]''\n', 'F-peak-launch (N)', tsc.launch_Fx.max, tsc.launch_Fy.max, tsc.launch_Fz.max);
% fprintf( '%20s : [%5.2f %5.2f %5.2f]''\n', 'M-peak-launch (N*m)', tsc.launch_Mx.max, tsc.launch_My.max, tsc.launch_Mz.max);
% fprintf( '\n');
fprintf( '%20s : %5.2f\n', 'v-mag-release (m/s)', tsc.v_inf.Data(idx_release));
fprintf( '%20s : %5.2f\n', 'r-release (rad/s)', tsc.r.Data(idx_release));
fprintf( '%20s : %5.2f\n', 'E-release (J)', tsc.ke_tot.Data(idx_release));
fprintf( '%20s : %5.2f\n', 'AdvR-release', tsc.AdvR.Data(idx_release));
fprintf( '%20s : %5.2f\n', 'alpha-release (deg)', tsc.alpha.Data(idx_release) .* (180/pi));
fprintf( '%20s : %5.2f\n', 'lift-release (N)', tsc.lift.Data(idx_release));
fprintf( '%20s : %5.2f\n', 'drag-release (N)', tsc.drag.Data(idx_release));

% X0_desired
% X0_huck
% X0_desire ./ X0_huck

%%
% title_str = sprintf( '[%4s %5.2f %4s %5.2f  %4s %5.2f] (m) [%4s %5.2f  %4s %5.2f  %4s %5.2f] (m/s)\n[%4s %5.1f  %4s %5.1f  %4s %5.1f] (rad) [%4s %5.1f  %4s %5.1f  %4s %5.1f] (rad/s)\n', ...
%     'x=', x0, 'y=', y0, 'z=', z0, ... 
%     'u=', u0, 'v=', v0, 'w=', w0, ...
%     '\phi=', phi0, '\theta=', theta0, '\psi=', psi0, ...
%     '\phi\prime=', phidot0, '\theta\prime=', thetadot0, '\psi\prime=', psidot0 );

%%
title_str = sprintf( '[%4s %5.2f %4s %5.2f  %4s %5.2f] (m) [%4s %5.2f  %4s %5.2f  %4s %5.2f] (m/s)\n[%4s %5.2f  %4s %5.2f  %4s %5.2f] (rad) [%4s %5.2f  %4s %5.2f  %4s %5.2f] (rad/s)\n', ...
    'x=', x0, 'y=', y0, 'z=', z0, ... 
    'u=', u0, 'v=', v0, 'w=', w0, ...
    '\phi=', phi0, '\theta=', theta0, '\psi=', psi0, ...
    '\phi\prime=', p0, '\theta\prime=', q0, '\psi\prime=', r0 );
%     '\phi=', phi0 * 180/pi, '\theta=', theta0 * 180/pi, '\psi=', psi0 * 180/pi, ...

%% trajectory
new_formatted_fig( 'trajectory');
hold on;
plot3( tsc.y.Data, tsc.x.Data, tsc.z_up.Data);
plot3(y0,x0,-z0,'*');
plot3( tsc.y.Data, tsc.x.Data, tsc.z_up.Data.*0, '-.');
plot3(y0,x0,0,'*');
hold off;
xlabel( 'N'); ylabel( 'E'); zlabel( 'U');
ylim( [-5 40]);
xlim( [-1 1] .* 40);
zlim( [0 7.5]);
grid on
title( title_str);
xlabel( sprintf( 'E\n%20s : %5.2f', 'E-release (J)', tsc.ke_tot.Data(idx_release)));
ylabel( sprintf( 'N\t%20s : %5.2f', 't-flight (s)', tsc.Time(end) - idx_release*0.001));
% view( -45, 20);
% view( -75, 10);
% view( -55, 35);
% view( -85, 15);
% view( -15, 80);
% view( -30, 20);
view( 10, 20);

snapnow;

% exportgraphics( gcf,'./wrk/trajectory.png','Resolution',300);
