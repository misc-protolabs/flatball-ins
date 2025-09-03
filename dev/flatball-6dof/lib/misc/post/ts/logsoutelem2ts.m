function ts = logsoutelem2ts( le)

% ts = timeseries( le.Values.Data, le.Values.Time, 'Name', le.Name);
ts = timeseries( le.Data, le.Time, 'Name', le.Name);
