function Idcti = DCTTransformInverse(Idct, A)
% I : input image
% A : DCT Matrix
    
    [L, C] = size(Idct);

    for i = 1:L
        for j = 1:C
            Idcti{i,j} = A'*double(Idct{i,j})*A;
        end
    end   
end