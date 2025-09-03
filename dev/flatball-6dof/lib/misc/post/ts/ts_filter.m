function y = ts_filter( x, tau)
%
% x - matlab timeseries
% dt - timestep
% tau - time constant of 1st order filter
%

y = x;

dt = mean( diff( y.Time));

two_dt_thr = 2*dt;

if tau < two_dt_thr
    tau = two_dt_thr;
    warning( sprintf( '(%5.3f < %5.3f) Inappropriate selection for time constant, setting to 2x the timestep...', tau, two_dt_thr));
end
a0 = dt / tau;
a1 = 1 - a0;

sz = size(y.Data);
for i=1:sz(2)
    y.Data(:,i) = filter( 1, [1/a0 -a1/a0], x.Data(:,i), mean( x.Data(1:100,i))*a1);
end
