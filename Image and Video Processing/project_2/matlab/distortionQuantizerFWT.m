function [PSNRfwt, d] = distortionQuantizerFWT(I, scale, scaling)
% I : I blocked
% A : DCT Matrix

    IFWT = FWT(I, scale, scaling);

    for k = 0:9
        IFWTq = uniformQuantizer(IFWT, 2^k);
        %IFWTi = FWTI(IFWTq, scale, scaling);
        d(k+1) = immse(IFWTq, IFWT);
        PSNRfwt(k+1) = 10.*log10(255^2 ./ d(k+1));
    end
end