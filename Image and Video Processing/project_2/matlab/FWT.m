function IFWT = FWT(I, scale, scaling)

for k = 1:scale

    [L, C] = size(I);

    for i = 1:L
        IFWT(i, 1:C) = AnalysisFilterBank(I(i, 1:C), scaling);
    end

    for j = 1:C
        IFWT(1:L, j) = AnalysisFilterBank(IFWT(1:L, j)', scaling);
    end


    I = IFWT(1:L/2, 1:C/2);

end

end