%% DCT-based Image Compression

close all
clear all

%I = imread('../images/peppers512x512.tif');
I = imread('../images/harbour512x512.tif');
%I = imread('../images/boats512x512.tif');

M = 8;

% DCT matrix
A = DCTMatrix(M);

%Blocked the image 
Iblock = MbyMblocked(I, M);


% DCT operation
Idct = DCTTransform(Iblock, A);


% DCT inverse operation
Idcti = DCTTransformInverse(Idct, A);

% Plots
subplot(1,3,1)
imshow(I)
title('Original image')
axis square

subplot(1,3,2)
imshow(uint8(cell2mat(Idct)));
title('DCT operation')
axis square

subplot(1,3,3)
imshow(uint8(cell2mat(Idcti)));
title('DCT inverse operation')
axis square

%% Uniform Quantizer

close all 
clear all

in = -10:0.01:10;

out = uniformQuantizer(in, 2);

plot(in, out);
title('Uniform mid-tread quantizer without thresh-old characteristic')
xlabel('input')
ylabel('output')


%% Distortion and Bit-Rate Estimation DCT

close all 
clear all

I1 = imread('../images/peppers512x512.tif');
I2 = imread('../images/harbour512x512.tif');
I3 = imread('../images/boats512x512.tif');

M = 8;
A = DCTMatrix(M);
Iblock = MbyMblocked(I1, M);

% distortion
[PSNR, MSE] = distortionQuantizer(Iblock, A);


Idct = DCTTransform(Iblock, A);

for k = 0:9
    Idctq = uniformQuantizer(cell2mat(Idct), 2^k);
    Rdct(k+1) = calculEntropy(Idctq, 2^k, M);
end
  
figure(1)
plot(Rdct, PSNR, '+-');
xlim([0 5]);
ylim([0 60]);
xlabel('Rate [bit/pixel]');
ylabel('PSNR [dB]');
title('Distortion vs Bit rate estimation')

Txdct = 100.*(1 - Rdct./8);

figure(2)
plot(Txdct, PSNR, '+-');
xlim([0 100]);
ylim([0 60]);
xlabel('Compression Ratio [%]');
ylabel('PSNR [dB]');
title('Distortion vs compression ratio estimation')



%% FWT-based Image Compression

clear all
close all

load('coeffs.mat');

%I = imread('../images/peppers512x512.tif');
I = imread('../images/harbour512x512.tif');
%I = imread('../images/boats512x512.tif');
%I = imread('../images/lena512.bmp');

scale = 4;
scaling = db4;

IFWT = FWT(I, scale, scaling);

IFWTi = FWTI(IFWT, scale, scaling);

subplot(1,3,1)
imshow(I)
title('Original image')
axis square

subplot(1,3,2)
imshow(uint8(IFWT))
title('DWT Operation')
axis square

subplot(1,3,3)
imshow(uint8(IFWTi))
title('DWT inverse operation')
axis square

%% Distortion and Bit-Rate Estimation FWT

clear all
close all

load('coeffs.mat');

I1 = imread('../images/peppers512x512.tif');
I2 = imread('../images/harbour512x512.tif');
I3 = imread('../images/boats512x512.tif');

scale = 4;
scaling = db4;

% distortion
[PSNR, MSE] = distortionQuantizerFWT(I1, scale, scaling);

IFWT = FWT(I1, scale, scaling);

for k = 0:9
    IFWTq = uniformQuantizer(IFWT, 2^k);
    
    IFWTqb{1,1} = IFWTq(1:32, 1:32);
    IFWTqb{2,1} = IFWTq(1:32, 33:64);
    IFWTqb{1,2} = IFWTq(33:64, 1:32);
    IFWTqb{2,2} = IFWTq(33:64, 33:64);
    
    IFWTqb{3,1} = IFWTq(1:64, 65:128);
    IFWTqb{4,1} = IFWTq(1:128, 129:256);
    IFWTqb{5,1} = IFWTq(1:256, 257:512);
    
    IFWTqb{1,3} = IFWTq(65:128, 1:64);
    IFWTqb{1,4} = IFWTq(129:256, 1:128);
    IFWTqb{1,5} = IFWTq(257:512, 1:256);
    
    IFWTqb{3,3} = IFWTq(65:128, 65:128);
    IFWTqb{4,4} = IFWTq(129:256, 129:256);
    IFWTqb{5,5} = IFWTq(257:512, 257:512);
    
    E(1) = calculEntropySimple(IFWTqb{1,1}, 2^k)/256;
    E(2) = calculEntropySimple(IFWTqb{1,2}, 2^k)/256;
    E(3) = calculEntropySimple(IFWTqb{2,1}, 2^k)/256;
    E(4) = calculEntropySimple(IFWTqb{2,2}, 2^k)/256;
    E(5) = calculEntropySimple(IFWTqb{1,3}, 2^k)/64;
    E(6) = calculEntropySimple(IFWTqb{1,4}, 2^k)/16;
    E(7) = calculEntropySimple(IFWTqb{1,5}, 2^k)/4;
    E(8) = calculEntropySimple(IFWTqb{3,1}, 2^k)/64;
    E(9) = calculEntropySimple(IFWTqb{4,1}, 2^k)/16;
    E(10) = calculEntropySimple(IFWTqb{5,1}, 2^k)/4;
    E(11) = calculEntropySimple(IFWTqb{3,3}, 2^k)/64;
    E(12) = calculEntropySimple(IFWTqb{4,4}, 2^k)/16;
    E(13) = calculEntropySimple(IFWTqb{5,5}, 2^k)/4;
    
    Rdwt(k+1) = sum(E);
    
end
  
plot(Rdwt, PSNR, '+-');
xlim([0 5]);
ylim([0 60]);
xlabel('Rate [bit/pixel]');
ylabel('PSNR [dB]');
title('Distortion vs Bit rate estimation')

Txdwt = 100.*(1 - Rdwt./8);

figure(2)
plot(Txdwt, PSNR, '+-');
xlim([0 100]);
ylim([0 60]);
xlabel('Compression Ratio [%]');
ylabel('PSNR [dB]');
title('Distortion vs compression ratio estimation')

%% Performance

clear all
close all

load('coeffs.mat');

I1 = imread('../images/peppers512x512.tif');
I2 = imread('../images/harbour512x512.tif');
I3 = imread('../images/boats512x512.tif');

I = I3;

% DCT
M = 8;
A = DCTMatrix(M);
Iblock = MbyMblocked(I, M);

% distortion
[PSNRdct, MSEdct] = distortionQuantizer(Iblock, A);


Idct = DCTTransform(Iblock, A);

for k = 0:9
    Idctq = uniformQuantizer(cell2mat(Idct), 2^k);
    Rdct(k+1) = calculEntropy(Idctq, 2^k, M);
end



% DWT
[PSNRdwt, MSEdwt] = distortionQuantizerFWT(I, 4, db4);

IFWT = FWT(I, 4, db4);

% computation entropy FWT. I didn't find an easy and general way to
% implement this algorithm.

for k = 0:9
    IFWTq = uniformQuantizer(IFWT, 2^k);
    
    IFWTqb{1,1} = IFWTq(1:32, 1:32);
    IFWTqb{2,1} = IFWTq(1:32, 33:64);
    IFWTqb{1,2} = IFWTq(33:64, 1:32);
    IFWTqb{2,2} = IFWTq(33:64, 33:64);
    
    IFWTqb{3,1} = IFWTq(1:64, 65:128);
    IFWTqb{4,1} = IFWTq(1:128, 129:256);
    IFWTqb{5,1} = IFWTq(1:256, 257:512);
    
    IFWTqb{1,3} = IFWTq(65:128, 1:64);
    IFWTqb{1,4} = IFWTq(129:256, 1:128);
    IFWTqb{1,5} = IFWTq(257:512, 1:256);
    
    IFWTqb{3,3} = IFWTq(65:128, 65:128);
    IFWTqb{4,4} = IFWTq(129:256, 129:256);
    IFWTqb{5,5} = IFWTq(257:512, 257:512);
    
    E(1) = calculEntropySimple(IFWTqb{1,1}, 2^k)/256;
    E(2) = calculEntropySimple(IFWTqb{1,2}, 2^k)/256;
    E(3) = calculEntropySimple(IFWTqb{2,1}, 2^k)/256;
    E(4) = calculEntropySimple(IFWTqb{2,2}, 2^k)/256;
    E(5) = calculEntropySimple(IFWTqb{1,3}, 2^k)/64;
    E(6) = calculEntropySimple(IFWTqb{1,4}, 2^k)/16;
    E(7) = calculEntropySimple(IFWTqb{1,5}, 2^k)/4;
    E(8) = calculEntropySimple(IFWTqb{3,1}, 2^k)/64;
    E(9) = calculEntropySimple(IFWTqb{4,1}, 2^k)/16;
    E(10) = calculEntropySimple(IFWTqb{5,1}, 2^k)/4;
    E(11) = calculEntropySimple(IFWTqb{3,3}, 2^k)/64;
    E(12) = calculEntropySimple(IFWTqb{4,4}, 2^k)/16;
    E(13) = calculEntropySimple(IFWTqb{5,5}, 2^k)/4;
    
    Rdwt(k+1) = sum(E);
    
end
  

figure(1)
plot(Rdwt, PSNRdwt, '+-'); hold on;
plot(Rdct, PSNRdct, '+-');
xlim([0 6]);
ylim([0 60]);
xlabel('Rate [bit/pixel]');
ylabel('PSNR');
legend('DWT (db4)', 'DCT');
title('Distortion vs Bit rate estimation')

% figure(2)
% plot([1 2 4 8 16 32 64 128 256 512], PSNRdwt, '+-'); hold on;
% plot([1 2 4 8 16 32 64 128 256 512], PSNRdct, '+-');
% xlim([0 512]);
% ylim([0 60]);
% xlabel('Quantization levels');
% ylabel('PSNR');
% legend('DWT (db4)', 'DCT');
% 
% 
% Txdwt = 100.*(1 - Rdwt./8);
% Txdct = 100.*(1 - Rdct./8);
% 
% figure(3)
% plot(Txdwt, PSNRdwt, '+-'); hold on;
% plot(Txdct, PSNRdct, '+-');
% xlim([0 100]);
% ylim([0 60]);
% xlabel('Taux de compression');
% ylabel('PSNR');
% legend('DWT (db4)', 'DCT');

%% Comparison images quantized


clear all
close all

load('coeffs.mat');

I1 = imread('../images/peppers512x512.tif');
I2 = imread('../images/harbour512x512.tif');
I3 = imread('../images/boats512x512.tif');

step = 512;

% DCT
M = 8;
[L, C] = size(I1);
A = DCTMatrix(M);
Iblock = MbyMblocked(I1, M);
Idct = DCTTransform(Iblock, A);
Idctq = uniformQuantizer(cell2mat(Idct), step);
Idcti = DCTTransformInverse(mat2cell(Idctq, M*ones(L/M, 1), M*ones(C/M, 1)), A);


IFWT = FWT(I1, 4, db4);
IFWTq = uniformQuantizer(IFWT, step);
IFWTi = FWTI(IFWTq, 4, db4);

subplot(1,2,1)
imshow(uint8(cell2mat(Idcti)))
axis square
title('DCT')

subplot(1,2,2)
imshow(uint8(IFWTi))
axis square
title('DWT')
