function DFT(x, N, S)
% x is a vector of speech, N is the frame length (must be even),
% S is the first sample of the frame to be analyzed

xf = x(S:S+N-1).*hanning(N);
%xf = x(S:S+N-1).*rectwin(N);
X = fft(xf);
%figure(1); clf; 
plot(0:44100/N:44100/2, 10*log10(abs(X(1:N/2+1)).^2)); hold on;
title('DFT of a sound region')
xlabel('frequency (Hz)')
ylabel('Power (dB)')

end