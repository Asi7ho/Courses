function [PSNR, d] = distortionQuantizer(I, range)
% I : I blocked
% A : DCT Matrix

    for k = range
        Idctq = uniformQuantizer(cell2mat(I), 2^k);
        %Idcti = DCTTransformInverse(mat2cell(Idctq, 8*ones(64, 1), 8*ones(64, 1)), A);
        d(k-(range(1))+1) = immse(Idctq, cell2mat(I));
        PSNR(k-(range(1))+1) = 10.*log10(255^2 ./ d(k-(range(1))+1));
    end
end