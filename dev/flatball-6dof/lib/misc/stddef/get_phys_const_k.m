
function val = misc_get_phys_const_k( nam)
%
% get physical constant - engineering value
%

val = evalin( 'base', ['C_' nam '_K']);
