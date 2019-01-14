function Idct = DCTTransform(Iblock, A)
% I : input image blocked
% A : DCT Matrix

    [L, C] = size(Iblock);

    for i = 1:L
        for j = 1:C
            Idct{i,j} = A*double(Iblock{i,j})*A';
        end
    end   
end