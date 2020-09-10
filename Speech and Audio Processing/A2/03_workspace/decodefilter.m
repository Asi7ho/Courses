function Aq = decodefilter(codeA, cb1, cb2)
    for i = 1:size(codeA, 1)
        
        idx1 = codeA(i, 1);
        idx2 = codeA(i, 2);
        
        lsf = cb1(idx1, :);
        res = cb2(idx2, :);
        
        Alsf = sort(lsf + res);
        
        Aq(i, :) = lsf2poly(Alsf);
    end
end