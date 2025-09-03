function y = ts_derrivative( x)

try
    y = x.copy;
catch
    y = x;
end

y.data(isnan(y.data)) = 0;

% ts.Data = cumsum(ts.Data) .* mean(diff(ts.Time));
y.Data(2:end) = diff(y.Data)./diff(y.Time);
y.Data(1) = 0;

