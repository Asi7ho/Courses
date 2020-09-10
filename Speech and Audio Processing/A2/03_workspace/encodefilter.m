function codeA = encodefilter(A, cb1, cb2)

for i = 1:length(A)
    
    lsf = poly2lsf(A(i,:))';
   
    dist = sqrt(sum((lsf - cb1).^2, 2));
    idx1 = find(dist == min(dist));
    
    res = lsf - cb1(idx1,:);
    dist = sqrt(sum((res - cb2).^2, 2));
    idx2 = find(dist == min(dist));
    
    codeA(i, :) = [idx1, idx2] ;
end

end