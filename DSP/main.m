
%% Task 2
clear all
close all

Fe = 48000;
order = 50;
fc = 3000;

d = fdesign.lowpass('n,fc',order,fc,Fe);
Hd = window(d,'window',@hamming);
%fvtool(Hd);

hL = Hd.Numerator;
[H,F] = freqz(hL,1,[],Fe);

subplot(2,1,1)
stem(0:length(hL)-1, hL, 'LineWidth', 2);
xlim([0 length(hL)-1])
xlabel('n', 'FontSize', 20); 
ylabel('h_L[n]', 'FontSize', 20);

subplot(2,1,2)
plot(F./Fe,20*log10(abs(H)), 'LineWidth', 2); hold on;
ylim([-120 0])
xlabel('\nu', 'FontSize', 20); 
ylabel('20log_{10} |H_L(\nu)|', 'FontSize', 20);

p1 = plot([1/8 0.5],[-40 -40], 'r', 'LineWidth', 2); hold on;
plot([1/8 1/8],[0  -40], 'r', 'LineWidth', 2); hold on;

p2 = plot([0 1/16],[-6 -6], 'g', 'LineWidth', 2); hold on;
plot([1/16 1/16],[0  -6], 'g', 'LineWidth', 2); hold on;

legend([p1 p2], {'\nu = 1/8 and y = -40dB', '\nu = 1/16 and y = -6dB'}, 'FontSize', 16)

export_filter(hL, 10, 'filter.txt');

%% Task 3

clear all
close all

Fe = 48000;
order = 26;
fc = 3000;

d = fdesign.lowpass('n,fc',order,fc,Fe);
Hd = window(d,'window',@hamming);
hL = Hd.Numerator;


m = (order+2)/2;

for n = 1:order+1
    if m == n
        hH(n) = 1 - hL(n);
    else
        hH(n) = dirac(n - m) - hL(n);
    end
end

[H,F] = freqz(hH,1,[],Fe);

subplot(2,1,1)
stem(0:length(hH)-1, hH, 'LineWidth', 2)
xlim([0 length(hH)-1])
xlabel('n', 'FontSize', 20); 
ylabel('h_H[n]', 'FontSize', 20);

subplot(2,1,2)
plot(F./Fe,20*log10(abs(H)), 'LineWidth', 2); hold on;
ylim([-120 10])
xlabel('\nu', 'FontSize', 20); 
ylabel('20log_{10} |H_H(\nu)|', 'FontSize', 20);

p1 = plot([0 1/16],[-6 -6], 'g', 'LineWidth', 2); hold on;
plot([1/16 1/16],[10  -6], 'g', 'LineWidth', 2); hold on;

legend([p1], {'\nu = 1/16 and y = -6dB'}, 'Location', 'southeast' , 'FontSize', 16)

%% Task 4

clear all
close all

clear all
close all

Fe = 48000;
order = 26;
fc = 3000;

d = fdesign.lowpass('n,fc',order,fc,Fe);
Hd = window(d,'window',@hamming);
hL = Hd.Numerator;

F = 10;

hQ = 2^(-F)*round(hL.*2^F);

[H0,F0] = freqz(hL,1,[],Fe);
[H,F] = freqz(hQ,1,[],Fe);


p1 = plot(F./Fe,20*log10(abs(H)), 'LineWidth', 2); hold on;
p2 = plot(F0./Fe,20*log10(abs(H0)), 'c--','LineWidth', 2); hold on;
ylim([-120 0])
xlabel('\nu', 'FontSize', 20); 
ylabel('20log_{10} |H_Q(\nu)|', 'FontSize', 20);

p3 = plot([1/8 0.5],[-40 -40], 'r', 'LineWidth', 2); hold on;
plot([1/8 1/8],[0  -40], 'r', 'LineWidth', 2); hold on;

p4 = plot([0 1/16],[-6 -6], 'g', 'LineWidth', 2); hold on;
plot([1/16 1/16],[0  -6], 'g', 'LineWidth', 2); hold on;

legend([p1 p2 p3 p4], {'Quantized filter', 'Original filter', '\nu = 1/8 and y = -40dB', '\nu = 1/16 and y = -6dB'}, 'FontSize', 16)

%% Task 5

clear all
close all

Fe = 48000;
order = 26;
fc = 3000;

d = fdesign.lowpass('n,fc',order,fc,Fe);
Hd = window(d,'window',@hamming);
hL = Hd.Numerator;

x = randi([-2048 2048], 1, 1e6);
xL = filter(hL, 1, x);

for F = 1:32
    hQ = 2^(-F)*round(hL.*2^(F));

    [H,Fr] = freqz(hQ,1,[],Fe);
    
    xQ = round(filter(hQ, 1, x));

    SQNR(F) = 10*log10(var(xL)/mse(xL, xQ));
end

figure(2)
plot(1:F, SQNR);
xlabel('F', 'FontSize', 20); 
ylabel('10log_{10}(SQNR(F))', 'FontSize', 20);
xlim([1 F])



