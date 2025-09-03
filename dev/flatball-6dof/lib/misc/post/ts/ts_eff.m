function y = ts_eff( ts1, ts2, lo, hi, dt, z, wn)


n = ts_filt( ts1, dt, z, wn, 0) ./ ts_filt( ts2, dt, z, wn, 0);

n = min( n, ones( length( n),1) * hi);
n = max( n, ones( length( n),1) * lo);

y = n;
