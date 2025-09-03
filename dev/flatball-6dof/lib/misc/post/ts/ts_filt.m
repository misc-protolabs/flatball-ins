function y = ts_filt( x, z, wn)

% z = sqrt(2)/2;
% wn = .25;
try
    y = x.copy;
catch
    y = x;
end

filt_2ord_lag = tf( wn^2, [1 2*z*wn wn^2]);
filt_1ord_lag = tf( wn, [1 wn]);
if z >= 0
    filt = filt_2ord_lag;
else
    filt = filt_1ord_lag;
end

% y_ = lsim( filt_2ord_lag, y.Data, y.Time);
% y_ = lsim( ss(filt_2ord_lag), y.Data, y.Time, [mean(y.Data) mean(y.Data)]);

% y_ = lsim( filt, y.Data, y.Time);
f = c2d( filt_1ord_lag, 0.01);
y_ = filter( f.num{1}, f.den{1}, y.Data, mean( y.Data(1:100)));

y.Data = y_;

% if show_plot
%     plot( ts_.Time, y);
% end
