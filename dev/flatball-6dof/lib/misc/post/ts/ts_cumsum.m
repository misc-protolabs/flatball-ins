

function ts = ts_cumsum(ts)

ts.data(isnan(ts.data)) = 0;
ts.data = cumsum(ts.data);

