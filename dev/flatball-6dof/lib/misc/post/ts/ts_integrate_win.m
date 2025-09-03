function y = ts_integrate_win( x, idx1, idx2)

try
    y = x.copy;
catch
    y = x;
end

% y.Data(isnan(y.Data)) = 0;
y = y .* 0;

tmp = cumtrapz( x.Time(idx1:idx2), x.Data(idx1:idx2));

y.Data(idx1:idx2) = tmp;
y.Data(idx2:end) = tmp(end);
