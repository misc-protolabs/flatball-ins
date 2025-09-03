function grid_on_all()

axesHandles = findobj('Type','axes');

for ii = 1:1:length(axesHandles)
    set(axesHandles(ii), 'XGrid', 'on');
    set(axesHandles(ii), 'YGrid', 'on');
end