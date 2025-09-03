function y = ts_integrate( x)

try
    y = x.copy;
catch
    y = x;
end

y.data(isnan(y.data)) = 0;

% ts.Data = cumsum(ts.Data) .* mean(diff(ts.Time));
y.Data = cumtrapz(y.Time, y.Data);
