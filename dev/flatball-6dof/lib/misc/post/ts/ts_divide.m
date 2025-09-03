function z = ts_divide( x, y)

z = x;

z.data(isnan(z.data)) = 0;

% ts.Data = cumsum(ts.Data) .* mean(diff(ts.Time));
z.Data = z.Data ./ y.Data;
