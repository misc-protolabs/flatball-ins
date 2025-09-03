
function create_phys_const( nam, val)
%
% create physical constant
%

try
    C = evalin( 'base', 'C');
catch
    C = struct;
end

C.(nam) = val;
assignin( 'base', 'C', C);
