clear all
close all

addpath('PattRecClasses-2')
addpath('files/GetMusicFeatures')
addpath('files/Songs')

%% Get audio && Observe audio

filename1 = 'melody_1.wav';
filename2 = 'melody_2.wav';
filename3 = 'melody_3.wav';
[y1, Fs1] = audioread(filename1);
[y2, Fs2] = audioread(filename2);
[y3, Fs3] = audioread(filename3);

t1 = 0:1/Fs1:length(y1)/Fs1 - 1/Fs1;
t2 = 0:1/Fs2:length(y2)/Fs2 - 1/Fs2;
t3 = 0:1/Fs3:length(y3)/Fs3 - 1/Fs3;

figure(1)
subplot(3, 1, 1)
plot(t1, y1);
title(filename1, 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Soundwave', 'FontSize', 18)

subplot(3, 1, 2)
plot(t2, y2);
title(filename2, 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Soundwave', 'FontSize', 18)

subplot(3, 1, 3)
plot(t3, y3);
title(filename3, 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Soundwave', 'FontSize', 18)
% t_start = 1.8 + 1/Fs;
% axis([t(t_start*Fs) t(t_start*Fs+200) min(y) max(y)])

%% Get features
winlength = 0.03;

frIsequence1 = GetMusicFeatures(y1, Fs1, winlength);
frIsequence2 = GetMusicFeatures(y2, Fs2, winlength);
frIsequence3 = GetMusicFeatures(y3, Fs3, winlength);

t_windowed1 = 0:winlength/2:(size(frIsequence1, 2) - 1)*winlength/2;
t_windowed2 = 0:winlength/2:(size(frIsequence2, 2) - 1)*winlength/2;
t_windowed3 = 0:winlength/2:(size(frIsequence3, 2) - 1)*winlength/2;

figure(2) % Pitch
subplot(3, 1, 1)
plot(t_windowed1, frIsequence1(1, :));
title(['Pitch of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Pitch estimate (Hz)', 'FontSize', 18)
set(gca,'YScale','log')

subplot(3, 1, 2)
plot(t_windowed2, frIsequence2(1, :));
title(['Pitch of ' filename2], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Pitch estimate (Hz)', 'FontSize', 18)
set(gca,'YScale','log')

subplot(3, 1, 3)
plot(t_windowed3, frIsequence3(1, :));
title(['Pitch of ' filename3], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Pitch estimate (Hz)', 'FontSize', 18)
set(gca,'YScale','log')


figure(3) % Intensity
subplot(3, 1, 1)
plot(t_windowed1, frIsequence1(3, :));
title(['Intensity of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Intensity estimate', 'FontSize', 18)
set(gca,'YScale','log')

subplot(3, 1, 2)
plot(t_windowed2, frIsequence2(3, :));
title(['Intensity of ' filename2], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Intensity estimate', 'FontSize', 18)
set(gca,'YScale','log')

subplot(3, 1, 3)
plot(t_windowed3, frIsequence3(3, :));
title(['Intensity of ' filename3], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Intensity estimate', 'FontSize', 18)
set(gca,'YScale','log')

%% Post Process / Extract Features

feature1 = PostProcess(frIsequence1);
feature2 = PostProcess(frIsequence2);
feature3 = PostProcess(frIsequence3);

figure(4)
subplot(3, 1, 1)
plot(t_windowed1, feature1);
title(['Features of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

subplot(3, 1, 2)
plot(t_windowed2, feature2);
title(['Features of ' filename2], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

subplot(3, 1, 3)
plot(t_windowed3, feature3);
title(['Features of ' filename3], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)


%% Test - Transposition

frIsequence1_t = [1.5*frIsequence1(1, :); frIsequence1(2, :); frIsequence1(3, :)];

feature1 = PostProcess(frIsequence1);
feature1_t = PostProcess(frIsequence1_t);

figure(5)
subplot(2, 1, 1)
plot(t_windowed1, feature1);
title(['Features of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

subplot(2, 1, 2)
plot(t_windowed1, feature1_t);
title(['Features of transposed ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

%% Test - Volume

v = 3:-2/length(y1):1+1/length(y1);
y1_t = v'.*y1;
frIsequence1_t = GetMusicFeatures(y1_t, Fs1, winlength);

feature1 = PostProcess(frIsequence1);
feature1_t = PostProcess(frIsequence1_t);

figure(6)
subplot(2, 1, 1)
plot(t_windowed1, feature1);
title(['Features of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

subplot(2, 1, 2)
plot(t_windowed1, feature1_t);
title(['Features of ' filename1 ' on a different volume'], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

%% Test - Pitch doubling/halving
N = 10;
idx = randi([1, size(frIsequence1, 2)], N, 1);
mul = rand(N, 1);
mul(mul < 0.5) = 0.5;
mul(mul > 0.5) = 2;

pitch_t = mul'.*frIsequence1(1, idx);

freq1_t = frIsequence1(1, :);
freq1_t(idx) = pitch_t;

frIsequence1_t = [freq1_t; frIsequence1(2, :); frIsequence1(3, :)];

feature1  = PostProcess(frIsequence1);
feature1_t  = PostProcess(frIsequence1_t);

figure(7)
subplot(2, 1, 1)
plot(t_windowed1, feature1);
title(['Features of ' filename1], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)

subplot(2, 1, 2)
plot(t_windowed1, feature1_t);
title(['Features of ' filename1 ' with pitch halving/doubling'], 'FontSize', 18);
xlabel('time (s)', 'FontSize', 18)
ylabel('Features', 'FontSize', 18)