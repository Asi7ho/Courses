function Ideblurred = deblurredWiener(I, h, variance)
% I = distored image
% h = blur function
% var = variance of the noise

[M, N] = size(I);

I = edgetaper(I, fspecial('gaussian',20,10));

If = fft2(I);
Hf = fft2(h, M, N);
K = variance/mean(I(:));


Ideblurredf = zeros(M,N);


for i = 1:M
    for j = 1:N
        Ideblurredf(i, j) = abs(Hf(i, j))^2*If(i, j)/((abs(Hf(i, j))^2 + K)*Hf(i, j));
    end
end


Ideblurred = uint8(ifft2(Ideblurredf));
Ideblurred = circshift(Ideblurred, [8 8]);