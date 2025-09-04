% Copyright 2025 Michael V. Schaefer
% 
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at:
% 
%     http://www.apache.org/licenses/LICENSE-2.0
% 
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.


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