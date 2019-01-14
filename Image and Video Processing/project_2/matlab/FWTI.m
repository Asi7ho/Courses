function IFWTi = FWTI(IFWT, scale, scaling)

[L, C] = size(IFWT);

for k = scale-1:-1:0
    
    Istk = IFWT(1:L/2^k, 1:C/2^k);

    [l, c] = size(Istk);

    for i = 1:l
        IFWTi(i, 1:l) = SynthesisFilterBank(Istk(i, 1:c), scaling);
    end
    
    Istk = IFWTi;
    
    for j = 1:c
        IFWTi(1:l, j) = SynthesisFilterBank(Istk(1:l, j)', scaling);
    end
    
    IFWT(1:l, 1:c) = IFWTi;


end

end