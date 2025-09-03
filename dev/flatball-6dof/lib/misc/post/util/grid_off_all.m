function grid_off_all()

axesHandles = findobj('Type','axes');

for ii = 1:1:length(axesHandles)
    set(axesHandles(ii), 'XGrid', 'off');
    set(axesHandles(ii), 'YGrid', 'off');
end