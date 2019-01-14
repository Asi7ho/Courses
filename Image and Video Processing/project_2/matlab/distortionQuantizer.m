function [PSNR, d] = distortionQuantizer(I, A)
% I : I blocked
% A : DCT Matrix

    Idct = DCTTransform(I, A);

    for k = 0:9
        Idctq = uniformQuantizer(cell2mat(Idct), 2^k);
        %Idcti = DCTTransformInverse(mat2cell(Idctq, 8*ones(64, 1), 8*ones(64, 1)), A);
        d(k+1) = immse(Idctq, cell2mat(Idct));
        PSNR(k+1) = 10.*log10(255^2 ./ d(k+1));
    end
end