
function q = create_phys_quantity( quant, min, max, res, sw_cnv_k)
%
% create physical quantity information
%

q = {};
q.name = quant;
q.eng_min = min;
q.eng_max = max;
q.eng_res = res;
q.sw_cnv_k = sw_cnv_k;

try
    Q = evalin( 'base', 'Q');
catch
    Q = struct;
end

Q.(quant) = q;
assignin( 'base', 'Q', Q);
