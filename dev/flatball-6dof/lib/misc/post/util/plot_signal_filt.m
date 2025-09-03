function plot_signal_filt( data, signal, y_m, y_b, tau, plotstyle, varargin)

if isfield( data, signal)
    x = ts_filter( data.(signal), tau);
    hold on
    h = plot(x.Time, x.Data .* y_m + y_b, plotstyle);
    if nargin > 1
        if rem(length(varargin), 2) == 0
            for ii = 2:2:length(varargin)
                set(h, varargin{ii-1}, varargin{ii});
            end
        end
    end
    hold off
    update_legend( signal);
end
    