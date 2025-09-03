function ts = ts_slope_offset( ts, m, b)

ts = (ts .* m) + b;
