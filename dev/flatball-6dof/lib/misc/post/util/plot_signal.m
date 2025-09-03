function pfUtil_plot_signal_f( data, signal, y_m, y_b, plotstyle, varargin)

flds = data.fieldnames;

if sum(cellfun(@(x) strcmp(x, signal), flds))
% if isfield( data, signal)
    hold on
    h = plot(data.(signal).Time, data.(signal).Data .* y_m + y_b, plotstyle);
    if nargin > 1
        if rem(length(varargin), 2) == 0
            for ii = 2:2:length(varargin)
                set(h, varargin{ii-1}, varargin{ii});
            end
        end
    end
    hold off
    pfUtil_update_legend_f( signal);
end
    