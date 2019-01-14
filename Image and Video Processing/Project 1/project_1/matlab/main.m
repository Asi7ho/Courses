%% Histogram equalization

clear all 
close all

I = imread('../images/lena512.bmp');

[L, C] = size(I);

a = 0.2;
b = 50;

% We concentrate the grey-levels around one area
for i = 1:L
    for j = 1:C
        I2(i, j) = min(max(round(a*I(i,j)+b), 0), 255);
    end
end

% Histogram equalization
I3 = histeq(I2);

figure(1)

subplot(3,2,1)
imshow(I)
title('lena512.bmp')

subplot(3,2,2)
histogram(I, 0:255)
xlim([0 255])
title('histogram Lena')

subplot(3,2,3)
imshow(I2)
title('lena512.bmp low-contrasted')

subplot(3,2,4)
histogram(I2, 0:255)
xlim([0 255])
title('histogram Lena low-contrasted')

subplot(3,2,5)
imshow(I3)
title('lena512.bmp low-contrasted then equalized')

subplot(3,2,6)
histogram(I3, 0:255)
xlim([0 255])
title('histogram Lena low-contrasted then equalized')

%% image denoising

clear all 
close all

I = imread('../images/lena512.bmp');

[L, C] = size(I);

% Gaussain noise
GaussianNoise = mynoisegen('gaussian', L, C, 0, 64);
IGaussian = I + uint8(GaussianNoise);

% Salt & pepper noise
SaPNoise = mynoisegen('saltpepper', L, C, 0.05, 0.05);
ISaP = I;
ISaP(SaPNoise == 0) = 0;
ISaP(SaPNoise == 1) = 255;


figure(1)

% subplot(3,2,1)
% imshow(I)
% title('lena512.bmp')
% 
% subplot(3,2,2)
% histogram(I, 0:255)
% title('histogram lena')
% xlim([0 255])

subplot(2,2,1)
imshow(IGaussian)
title('lena512.bmp + gaussian noise')

subplot(2,2,2)
histogram(IGaussian, 0:255)
title('histogram lena512.bmp + gaussian noise')
xlim([0 255])

subplot(2,2,3)
imshow(ISaP)
title('lena512.bmp + salt & pepper')

subplot(2,2,4)
histogram(ISaP, 0:255)
title('histogram lena512.bmp + salt & pepper')
xlim([0 255])





% Mean Filtering
MeanFilter = 1/9*ones(3,3);
IMean1 = uint8(conv2(IGaussian, MeanFilter, 'same'));
IMean2 = uint8(conv2(ISaP, MeanFilter, 'same'));

figure(2)

% subplot(3,2,1)
% imshow(I)
% title('lena512.bmp')
% 
% subplot(3,2,2)
% histogram(I, 0:255)
% title('histogram lena')
% xlim([0 255])

subplot(2,2,1)
imshow(IMean1)
title('lena512.bmp + gaussian noise + mean filter')

subplot(2,2,2)
histogram(IMean1, 0:255)
title('histogram lena512.bmp + gaussian noise + mean filter')
xlim([0 255])

subplot(2,2,3)
imshow(IMean2)
title('lena512.bmp + salt & pepper + mean filter')

subplot(2,2,4)
histogram(IMean2, 0:255)
title('histogram lena512.bmp + salt & pepper + mean filter')
xlim([0 255])





% Median Filtering
IEnlargedG = zeros(L+2, C+2);
IEnlargedSP = zeros(L+2, C+2);

IEnlargedG(2:L+1, 2:C+1) = IGaussian;
IEnlargedSP(2:L+1, 2:C+1) = ISaP;

for i = 2:L+1
    for j = 2:C+1
        Filter = IEnlargedG(i-1:i+1, j-1:j+1);
        SortedFilter = Filter(:);
        SortedFilter = sort(SortedFilter);
        IGFiltered(i, j) = uint8(SortedFilter(5));
    end
end

IGFiltered(:,1) = [];
IGFiltered(1,:) = [];

for i = 2:L+1
    for j = 2:C+1
        Filter = IEnlargedSP(i-1:i+1, j-1:j+1);
        SortedFilter = Filter(:);
        SortedFilter = sort(SortedFilter);
        ISaPFiltered(i, j) = uint8(SortedFilter(5));
    end
end

ISaPFiltered(:,1) = [];
ISaPFiltered(1,:) = [];


figure(3)

% subplot(3,2,1)
% imshow(I)
% title('lena512.bmp')
% 
% subplot(3,2,2)
% histogram(I, 0:255)
% title('histogram lena')
% xlim([0 255])

subplot(2,2,1)
imshow(IGFiltered)
title('lena512.bmp + gaussian noise + median filter')

subplot(2,2,2)
histogram(IGFiltered, 0:255)
title('histogram lena512.bmp + gaussian noise + median filter')
xlim([0 255])

subplot(2,2,3)
imshow(ISaPFiltered)
title('lena512.bmp + salt & pepper + median filter')

subplot(2,2,4)
histogram(ISaPFiltered, 0:255)
title('histogram lena512.bmp + salt & pepper + median filter')
xlim([0 255])



%% Frequency domain filtering

clear all 
close all

I = imread('../images/lena512.bmp');

[L, C] = size(I);

% Blurring
h = myblurgen('gaussian', 8);
IBlurred = uint8(conv2(I, h, 'same'));

figure(1)

subplot(2,2,1)
imshow(I);
title('lena512.bmp')

subplot(2,2,2)
FFT = uint8(fftshift(20*log10(abs(fft2(I)))));
imshow(FFT);
title('FT lena512.bmp')

subplot(2,2,3)
imshow(IBlurred);
title('lena512.bmp blurred')

subplot(2,2,4)
FFT2 = uint8(fftshift(20*log10(abs(fft2(IBlurred)))));
imshow(FFT2);
title('FT lena512.bmp blurred')

% Noise
INoise = IBlurred - I;
Var = norm(double(INoise(:)))/(L*C);

% Sharpening
[IDebblurred, gamma] = deblurred(IBlurred, h, Var);
IDebblurredW = deblurredWiener(IBlurred, h, Var);

figure(2)

% subplot(2,3,1)
% imshow(I);
% title('lena512.bmp')

subplot(2,2,1)
imshow(IBlurred);
title('lena512.bmp blurred')

subplot(2,2,2)
imshow(IDebblurred);
title('lena512.bmp deblurred with CLS')

% subplot(2,3,4)
% imshow(I);
% title('lena512.bmp')

subplot(2,2,3)
imshow(IBlurred);
title('lena512.bmp blurred')

subplot(2,2,4)
imshow(IDebblurred);
title('lena512.bmp deblurred with Wiener')

%% Test on out of focus images

clear all 
close all

% Tests on out of focus images

I1 = imread('../images/boats512_outoffocus.bmp');
I2 = imread('../images/man512_outoffocus.bmp');

h = myblurgen('gaussian', 8);

[IDebblurred1, gamma1] = deblurred(I1, h, 0.0833);
[IDebblurred2, gamma2] = deblurred(I2, h, 0.0833);

% IDebblurred1 = deblurredWiener(I1, h, 0.0833);
% IDebblurred2 = deblurredWiener(I2, h, 0.0833);

figure(1)

subplot(2,2,1)
imshow(I1)
title('boats512\_outoffocus.bmp')

subplot(2,2,2)
imshow(IDebblurred1);
title('boats512\_outoffocus.bmp deblurred')

subplot(2,2,3)
imshow(I2)
title('man512\_outoffocus.bmp')

subplot(2,2,4)
imshow(IDebblurred2);
title('man512\_outoffocus.bmp deblurred')