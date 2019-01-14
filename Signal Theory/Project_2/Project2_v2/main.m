%% Pre assigment

clear all;
close all;

I = imread('kth.jpg');
load('training.mat');

[key, cPic] = encoder(I);

keyF = filter([1 0.7 0.7 0.7], 1 , key);

min = 10;
order = 0;

for L = 0:31

    [X,R] = corrmtx(keyF(1:32),L,'autocorrelation');
    r = xcorr(training, keyF(1:32));
    r = r(32:32+L);
    
    w = R\r;
    w =  w./max(w);
    tabw(1:length(w), L+1) = w;
    
    keyR = filter(w , 1, keyF);
    keyD = sign(keyR);

    tabMSE1(L+1) = mse(keyR(1:32), training); %we compare every bits
    tabMSE2(L+1) = mse(keyD(1:32), training); %we compare after the treshold
    tabMSE3(L+1) = mean(training(L+1:32).^2) - r'*inv(R)*r; %formula MSE
    tabMSE4(L+1) = mse(keyR(L+1:32), training(L+1:32)); %we compare the right bits

end

keyR = filter(w(1:14) , 1, keyF);
keyD = sign(keyR);


dPic = decoder(keyD, cPic);

figure(1)
subplot(1,2,1)
image(I);
axis square;

subplot(1,2,2)
image(dPic);
axis square;

figure(2)
subplot(1,2,1)
plot(tabMSE1);

subplot(1,2,2)
plot(tabMSE2);

%% Assigment

clear all;
close all;

load('training.mat');
load('spydata.mat');

keyF = received;

for L = 0:31

    [X,R] = corrmtx(keyF(1:32), L,'autocorrelation');
    r = xcorr(training, keyF(1:32));
    r = r(32:32+L);
    
    w = R\r;
    w = w./max(w);
    tabw(1:length(w), L+1) = w;
    
    keyR = filter(w , 1, keyF);
    keyD = sign(keyR);

    tabMSE1(L+1) = mse(keyR(1:32), training);
    tabMSE2(L+1) = mse(keyD(1:32), training);
    tabMSE3(L+1) = mean(training(L+1:32).^2) - r'*inv(R)*r;
    tabMSE4(L+1) = mse(keyR(L+1:32), training(L+1:32));
    tabSNR(L+1) = psnr(keyR(L+1:32), training(L+1:32));
    
end

keyR = filter(tabw(:,14) , 1, keyF);
keyD = sign(keyR);

N = 1500;

bitError = randi([1 length(keyD)], [N, 1]);
keyD2 = keyD;
for i = 1:N
   keyD2(bitError(i)) = -keyD2(bitError(i));
end

dPic = decoder(keyD, cPic);

figure(1)
image(dPic);
axis square;

figure(2)
subplot(1,3,1)
plot(0:L, tabMSE4);
xlabel('filter order L', 'FontSize', 16);
ylabel('MSE before detection', 'FontSize', 16)

figure(2)
subplot(1,3,2)
plot(0:L, tabSNR);
xlabel('filter order L', 'FontSize', 16);
ylabel('SNR', 'FontSize', 16)

subplot(1,3,3)
plot(0:L, tabMSE2);
xlabel('filter order L', 'FontSize', 16);
ylabel('MSE after detection', 'FontSize', 16)


dPic2 = decoder(keyD2, cPic);
figure(3)
subplot(1,2,1)
image(dPic);
axis square;
subplot(1,2,2)
image(dPic2);
axis square;

