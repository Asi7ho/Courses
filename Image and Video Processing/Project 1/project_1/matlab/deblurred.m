function [Ideblurred, gamma] = deblurred(I, h, variance)
% I = distored image
% h = blur function
% var = variance of the noise

[M, N] = size(I);

I = edgetaper(I, fspecial('gaussian',20,10));

If = fft2(I);
Hf = fft2(h, M, N);
Hfc = conj(Hf);
gamma = 1e-5;
a = 0.05;
step = 1e-6;

p = [0 -1 0; -1 4 -1; 0 -1 0];

Pf = fft2(p, M, N);

Ideblurredf = zeros(M,N);

flag = false;

 while (~flag) && 1

    for i = 1:M
        for j = 1:N
            Ideblurredf(i, j) = Hfc(i, j)*If(i, j)/(abs(Hf(i, j))^2 + gamma*abs(Pf(i, j))^2);
        end
    end


    for i = 1:M
        for j = 1:N
            R(i, j) = If(i, j) - Hf(i, j)*Ideblurredf(i, j);
        end
    end

    r = ifft2(R);
    r2 = var(r(:));
    
    if r2 - variance < -a
        gamma = gamma + step;
    elseif r2 - variance > a
        gamma = gamma - step;
    else 
        flag = true;
        break;
    end
end

Ideblurred = uint8(ifft2(Ideblurredf));
Ideblurred = circshift(Ideblurred, [8 8]);
end