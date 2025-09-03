
function val = get_phys_const_eng( nam)
%
% get physical constant - engineering value
%

sw_val = evalin( 'base', ['C_' nam]);
sw_k = evalin( 'base', ['C_' nam(1:end-4) '_K']);
val = sw_val / sw_k;
% val = get_phys_const( nam) / get_phys_const_k( nam);
