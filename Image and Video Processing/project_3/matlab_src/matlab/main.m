%% Read Video

clear all 
close all

path = '../../foreman_qcif/foreman_qcif.yuv';
%path = '../../mother-daughter_qcif/mother-daughter_qcif.yuv';

Movie = yuv2mov(path);

ReadVideo(Movie, 50);

%% Intra-Frame Video Coder Video

clear all 
close all

path = '../../foreman_qcif/foreman_qcif.yuv';
%path = '../../mother-daughter_qcif/mother-daughter_qcif.yuv';

Luminance = yuv_import_y(path, [176 144], 50);
M = 8;
A = DCTMatrix(M);
range = 3:6;

% Encoder
for k = range
    for l = 1:size(Luminance, 1)
        I = Luminance{l, 1};
        Intra = IntraFrame(I, A, M);
        Intraq = uniformQuantizer(cell2mat(Intra), 2^k);
        BuffIntra{l, k-(range(1))+1} = Intraq;
    end
end


% Decoder
for k = range
    for l = 1:size(Luminance, 1)
        I = BuffIntra{l, k-(range(1))+1};
        DecodeIntra = DecodeIntraFrame(I, A, M);
        Decoder{l, k-(range(1))+1} = cell2mat(DecodeIntra);
    end
end

% Rate-bit
for k = range
    for l = 1:size(BuffIntra, 1)
        Rintra(l, k-(range(1))+1) = calculEntropy(BuffIntra{l, k-(range(1))+1}, 2^k, M);
    end
end
Rintra = mean(Rintra);
bpsIntra = Rintra.*size(I, 1).*size(I, 2).*30./1000;


% PSNR
for k = range
    for l = 1:size(Luminance, 1)
        MSE(l, k-(range(1))+1) = immse(Decoder{l, k-(range(1))+1}, Luminance{l, 1});
        PSNRintra(l, k-(range(1))+1) = 10.*log10(255^2 ./ MSE(l, k-(range(1))+1));
    end
end
PSNRintra = mean(PSNRintra);

% figure(1)
% plot(Rintra, PSNRintra, '+-');
% xlim([0 8]);
% ylim([0 60]);
% xlabel('Rate [bit/pixel]');
% ylabel('PSNR [dB]');
% title('Distortion vs Bit rate estimation')

figure(2)
plot(bpsIntra, PSNRintra, '+-');
xlabel('Bit-Rate [kbit/s]');
ylabel('PSNR [dB]');
title('Distortion vs Bit-rate estimation')

save('RateIntra.mat', 'Rintra');
save('bpsIntra.mat', 'bpsIntra');
save('PSNRIntra.mat', 'PSNRintra');


%% Conditional Replenishment Video Coder

clear all 
close all

path = '../../foreman_qcif/foreman_qcif.yuv';
%path = '../../mother-daughter_qcif/mother-daughter_qcif.yuv';

Luminance = yuv_import_y(path, [176 144], 50);
M = 8;
A = DCTMatrix(M);
range = 3:6;


% Intra Encoder
for k = range
    for l = 1:size(Luminance, 1)
        I = Luminance{l, 1};
        Intra = IntraFrame(I, A, M);
        Intraq = MbyMblocked(uniformQuantizer(cell2mat(Intra), 2^k), 16);
        BuffIntra{l, k-(range(1))+1} = Intraq;
    end
end

L = LengthBuff(BuffIntra, range);

% Copy/Intra Encoder
[Buff, CoI] = ConditionalFrame(Luminance, A, M, range, L);

%Decoder
DecodeCopy = DecodeConditionalFrame(Buff, A, M, range);


% Bit-Rate
for k = range
    for l = 1:size(Buff, 1)
        for i = 1:size(Buff{l, k-(range(1))+length(range)+1}, 1)
            for j = 1:size(Buff{l, k-(range(1))+length(range)+1}, 2)
                if Buff{l, k-(range(1))+length(range)+1}(i, j) == 0
                    H(i, j) = 1/256;
                else
                    H(i, j) = sum(sum(L{l, k-(range(1))+1}{i, j}))/256 + 1/256;
                end
            end
        end
        Rcopy(l, k-(range(1))+1) = mean2(H);
    end
end
Rcopy = mean(Rcopy);
bpsCopy = Rcopy.*(176*144*30/1000);


% PSNR
for k = range
    for l = 1:size(DecodeCopy, 1)
        MSE(l, k-(range(1))+1) = immse(DecodeCopy{l, k-(range(1))+1}, Luminance{l, 1});
        PSNRcopy(l, k-(range(1))+1) = 10.*log10(255^2 ./ MSE(l, k-(range(1))+1));
    end
end
PSNRcopy = mean(PSNRcopy);

figure(1)
plot(bpsCopy, PSNRcopy, '+-');
xlabel('Bit-Rate [kbit/s]');
ylabel('PSNR [dB]');
title('Distortion vs Bit-rate estimation')

save('RateCopy.mat', 'Rcopy');
save('bpsCopy.mat', 'bpsCopy');
save('PSNRcopy.mat', 'PSNRcopy');

%% Video Coder with Motion Compensation

clear all 
close all

path = '../../foreman_qcif/foreman_qcif.yuv';
%path = '../../mother-daughter_qcif/mother-daughter_qcif.yuv';

Luminance = yuv_import_y(path, [176 144], 50);
M = 8;
A = DCTMatrix(M);
range = 3:6;

% Intra Encoder
for k = range
    for l = 1:size(Luminance, 1)
        I = Luminance{l, 1};
        Intra = IntraFrame(I, A, M);
        Intraq = MbyMblocked(uniformQuantizer(cell2mat(Intra), 2^k), 16);
        BuffIntra{l, k-(range(1))+1} = Intraq;
    end
end

Lintra = LengthBuff(BuffIntra, range);

% Motion Compensated Encoder
MotionX = -10:10;
MotionY = -10:10;

Dict = DictMov(MotionX, MotionY);

[BuffResiduals, DxDy, Lmv, Lres] = MotionEncoder(Luminance, Dict, range, A, M, MotionX, MotionY);


%Inter Encoder
[Buff, CoIoM] = InterFrame(Luminance, A, M, range, BuffResiduals, DxDy, Dict, Lres, Lmv, Lintra);

%Inter Decoder
DecodeInter = DecodeInterFrame(Buff, A, M, range);

% Bit-Rate
for k = range
    for l = 1:size(Buff, 1)
        for i = 1:size(Buff{l, k-(range(1))+length(range)+1}, 1)
            for j = 1:size(Buff{l, k-(range(1))+length(range)+1}, 2)
                if Buff{l, k-(range(1))+length(range)+1}(i, j) == 0
                    H(i, j) = 2/256;
                elseif Buff{l, k-(range(1))+length(range)+1}(i, j) == 1
                    H(i, j) = sum(sum(Lintra{l, k-(range(1))+1}{i, j}))/256 + 2/256;
                else
                    H(i, j) = sum(sum(Lres{l-1, k-(range(1))+1}{i, j}))/256 + (Buff{l, k-(range(1))+length(range)+1}(i, j) - 2)/256 + 2/256;
                end
            end
        end
        Rinter(l, k-(range(1))+1) = mean2(H);
    end
end
Rinter = mean(Rinter);
bpsInter = Rinter.*(176*144*30/1000);


% PSNR
for k = range
    for l = 1:size(DecodeInter, 1)
        MSE(l, k-(range(1))+1) = immse(DecodeInter{l, k-(range(1))+1}, Luminance{l, 1});
        PSNRinter(l, k-(range(1))+1) = 10.*log10(255^2 ./ MSE(l, k-(range(1))+1));
    end
end
PSNRinter = mean(PSNRinter);

figure(1)
plot(bpsInter, PSNRinter, '+-');
xlabel('Bit-Rate [kbit/s]');
ylabel('PSNR [dB]');
title('Distortion vs Bit-rate estimation')

save('RateInter.mat', 'Rinter');
save('bpsInter.mat', 'bpsInter');
save('PSNRinter.mat', 'PSNRinter');


%% Performances

% clear all
% close all

load('RateIntra.mat');
load('bpsIntra.mat');
load('PSNRIntra.mat');

load('RateCopy.mat');
load('bpsCopy.mat');
load('PSNRcopy.mat');

load('RateInter.mat');
load('bpsInter.mat');
load('PSNRinter.mat');


plot(bpsIntra, PSNRintra, '+-'); hold on
plot(bpsCopy, PSNRcopy, '+-'); hold on
plot(bpsInter, PSNRinter, '+-');
xlabel('Bit-Rate [kbit/s]');
ylabel('PSNR [dB]');
title('Distortion vs Bit-rate estimation')
legend('Intra', 'Copy+Intra', 'Inter+Copy+Intra')


