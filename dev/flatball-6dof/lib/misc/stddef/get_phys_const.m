
function val = get_phys_const( nam)
%
% get physical constant
%

val = evalin( 'base', ['C.' nam]);
