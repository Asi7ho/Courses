clear all 
close all

load('assignment2.mat');

Fs = 8000;
y = speech8;

%% 2 The Uniforn Scalar Quantizer

x = -6:0.01:6;
n_bits = 2;
xmax = 4;


% Midrise quantizer
m = 0;

idx = sq_enc(x, n_bits, xmax, m);
outq = sq_dec(idx, n_bits, xmax, m);

figure(1);
subplot(1, 2, 1)
plot(x, x); hold on;
plot(x, outq)
legend('x', 'Quantized x')
title('Uniform midrise quantizer', 'FontSize', 18)


% Midtread quantizer
m = 1.5;

idx = sq_enc(x, n_bits, xmax, m);
outq = sq_dec(idx, n_bits, xmax, m);

subplot(1, 2, 2)
plot(x, x); hold on;
plot(x, outq)
legend('x', 'Quantized x')
title('Uniform midtread quantizer', 'FontSize', 18)

%% 3.1 Parametric Coding of Speech

ulen = 128;
[E, V, A, P] = analysis(y, 256, ulen, 10);

% linear
figure(2);
h_lin = histogram(E);
m_lin = mean(h_lin.BinEdges);
xmax_lin = max(h_lin.BinEdges) - m_lin;
n_bits_lin = 8;

idx = sq_enc(E, n_bits_lin, xmax_lin, m_lin);
QE = sq_dec(idx, n_bits_lin, xmax_lin, m_lin);

figure(3)
plot(E); hold on; plot(QE);
legend('Energy', 'Quantized linear-Energy')

[x_lin] = synthesis(QE, V, A, P, ulen);

% log
E_log = log(E);
figure(4)
h_log = histogram(E_log);

m_log = mean(h_log.BinEdges);
xmax_log = max(h_log.BinEdges) - m_log;
n_bits_log = 4;

idx = sq_enc(E_log, n_bits_log, xmax_log, m_log);
QE_log = sq_dec(idx, n_bits_log, xmax_log, m_log);

figure(5)
plot(E); hold on; plot(exp(QE_log));
legend('Energy', 'Quantized log-Energy')

[x_log] = synthesis(exp(QE_log), V, A, P, ulen); % log is better for bit-rate

%% 3.2 Quantizing the Pitch and Voiced/Unvoiced Decision
% Voice/Unvoice is a binary decisiion so 1 bit is sufficient
% Pitch is good with either linear or log quantization -> use log to stay
% coherent

% Voice/Unvoice quantizer
Vq = V;

% Pitch quantizer
P_log = log(P);
figure(6)
h_pitch_log = histogram(P_log);

m_pitch_log = mean(h_pitch_log.BinEdges);
xmax_pitch_log = max(h_pitch_log.BinEdges) - m_pitch_log;
n_bits_pitch_log = 5;

idx = sq_enc(P_log, n_bits_pitch_log, xmax_pitch_log, m_pitch_log);
QP_log = sq_dec(idx, n_bits_pitch_log, xmax_pitch_log, m_pitch_log);

figure(7)
plot(P); hold on; plot(exp(QP_log));
legend('Pitch', 'Quantized log-Pitch')

[x_pitch_log] = synthesis(E, V, A, exp(QP_log), ulen);

%% 3.3 Quantizing the LP parameters

codeA = encodefilter(A, lsfCB1, lsfCB2);
Aq = decodefilter(codeA, lsfCB1, lsfCB2);

%% 3.4 Optimizing the Bit Allocation

[xq] = synthesis(exp(QE_log), Vq, Aq, exp(QP_log), ulen);

SNR = 10*log10(var(y(1:length(xq)))/cov(y(1:length(xq)) - xq));

% 4 bits for Gain
% 5 bits for Pitch
% 1 bit for voice/unvoice
% 20 bits for LP

bpsp = (n_bits_log + n_bits_pitch_log + 1 + 20)/ulen; % bit per sample
bps = (n_bits_log + n_bits_pitch_log + 1 + 20)*Fs/ulen; % bit per second

%% 4.1 Uniform Scalar Quantization of Speech

% Determining the optimal k
n_bits = 3;
m = 0;

k = 0:0.01:12;

for i = 1:length(k)

xmax = k(i)*sqrt(var(y));

idx = sq_enc(y, n_bits, xmax, m);
outq = sq_dec(idx, n_bits, xmax, m);

SNR_USQ(i) = 10*log10(var(y)/cov(y - outq));

end

figure(8)
plot(k, SNR_USQ)
[~, id_opt] = max(SNR_USQ);
k_opt = k(id_opt);
xlabel('k', 'FontSize', 18)
ylabel('SNR', 'FontSize', 18)
title('Determination of k', 'FontSize', 18)


% Plotting the SNR-Rate curve
n_bits = 1:16;
k_opts = [0.48, 1.56, k_opt, 4.95, 6.3, 7.65, 8.85, ...
    9.95, 10.6, 11.0, 11.1, 11.2, 11.15, 11.2, 11.15, 11.15];

for i = 1:length(k_opts)
    xmax = k_opts(i)*sqrt(var(y));
    
    idx = sq_enc(y, n_bits(i), xmax, m);
    outq = sq_dec(idx, n_bits(i), xmax, m);
    
    SNR_opts(i) = 10*log10(var(y)/cov(y - outq));
end

figure(9)
plot(1:16, SNR_opts); hold on;
plot(1:16, 10*log10(2.^(2.*n_bits)));
xlabel('Rate', 'FontSize', 18)
ylabel('SNR', 'FontSize', 18)
legend({'experiment', 'theoretical'}, 'FontSize', 18)
title('SNR-Rate curve', 'FontSize', 18)

% At n_bits = 8, I cannot tell a different between the original and the
% quantized signal


% At R = 1, the noise is highly correlated with the original signal, it
% contains a lot of information
% At R = 12, we can hear a white noise, there is no correlation with the
% original signal


% At low rate, it is better to not have a reconstruction level in the origin for low rates
% The reconstructed signal is more intelligible and we have less noise in
% it

% Using a midtread USQ (optional)
for i = 1:3
    xmax = k_opts(i)*sqrt(var(y));
    m = xmax/2^n_bits(i);
    idx = sq_enc(y, n_bits(i), xmax, m);
    outq = sq_dec(idx, n_bits(i), xmax, m);
    
    SNR_opts(i) = 10*log10(var(y)/cov(y - outq));
end

%% Adaptive Open-Loop DPCM

alen = 256;
ulen = alen;

[A, E, d] = encoder(y, alen, ulen, 10);

% Use Gain E to adapt
E_log = log(E);
figure(4)
h_log = histogram(E_log);

m_log = mean(h_log.BinEdges);
xmax_log = max(h_log.BinEdges) - m_log;
n_bits_log = 4;

idx = sq_enc(E_log, n_bits_log, xmax_log, m_log);
QE_log = sq_dec(idx, n_bits_log, xmax_log, m_log);

codeA = encodefilter(A, lsfCB1, lsfCB2);
Aq = decodefilter(codeA, lsfCB1, lsfCB2);

n_bits = 3;

xmax = 0.5*sqrt(var(d));
m = xmax/2^n_bits; % use a mid-rise quantizer because of low rate

idx = sq_enc(d, n_bits, xmax, m);
outq = sq_dec(idx, n_bits, xmax, m);


[xhat] = decoder(Aq, exp(QE_log), outq, ulen);

figure(10)
pwelch(outq) %DFT quantized error

figure(11)
[Py, F] = pwelch(y(1:ulen), [],[],[],Fs,'onesided'); %DFT frame
[Pd, F] = pwelch(d(1:ulen), [],[],[],Fs,'onesided'); %DFT error frame

plot(F, 10*log10(Py)); hold on;
plot(F, 10*log10(Pd));
xlabel('Frequency (Hz)', 'FontSize', 18)
ylabel('Power/frequency (dB/Hz)', 'FontSize', 18)
legend({'PSD one frame input', 'PSD same frame residual'}, 'FontSize', 18)
title('Noise shapping', 'FontSize', 18)

% According to the theory we should have the same shape that the speech for
% the reconstruction error (noise shaping)

SNR_DPCM = 10*log10(var(y(1:length(xhat)))/cov(y(1:length(xhat)) - xhat));
SNR_PCM = 10*log10(var(d)/cov(d - outq));


RatePerSample = (20 + n_bits_log + n_bits*ulen)/ulen;
RatePerSecond = RatePerSample*Fs;

% For the SNR it is better to use the unquantized LP coefficients
% By listening there is no difference when using the quantized LP coefficients
% so to reduce the bit rate it is better to use the quantized LP
% coefficients
