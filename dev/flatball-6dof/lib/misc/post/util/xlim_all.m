function xlim_all(range)

axesHandles = findobj('Type','axes', '-not', 'Tag', 'legend','-not','Tag','Colorbar');

for ii = 1:1:length(axesHandles)
    set(axesHandles(ii), 'XLim', range);
end




% %% Newer way to do this??
% 
% figHandles = sort(findobj('Type','figure'));
% 
% for ii = 1:1:length(figHandles)
% %     figure(figHandles(ii));
%     allAxesInFigure = findall(figHandles(ii),'type','axes');
% 
%     for jj = 1:1:length(allAxesInFigure)
%         set(allAxesInFigure(jj), 'XLim', range);
%     end
% end
% 
% % ax = findobj(gcf,'type','axes','-not','Tag','legend','-not','Tag','Colorbar');
% 
% % isAxes = strcmp('axes',get(listOfHandles,'type'));