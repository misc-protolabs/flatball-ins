function y = ts_diff( x)

y = x;

y.data(isnan(y.data)) = 0;


% ts.Data = cumsum(ts.Data) .* mean(diff(ts.Time));
y.Data(2:end) = diff(y.Data);
y.Data(1) = 0;

