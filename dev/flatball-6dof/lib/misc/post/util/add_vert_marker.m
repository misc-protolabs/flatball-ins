function add_vert_marker( t_marker)

yl = ylim;
hold all;
plot( [1 1] .* t_marker, [-1 1].*1e4, '-.k');
% plot( [1 1].*tsc.Time(idx_launch), [-1 1].*1e4, '-.');
% plot( [1 1].*tsc.Time(idx_impact), [-1 1].*1e4, '-.');
ylim( yl);
hold off;
