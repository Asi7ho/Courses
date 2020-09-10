function plotmyspectrogram(x, alen, ulen)
% x is the speech vector
% alen is the analysis frame length, ulen is the update length
N = length(x);
naf = floor((N-alen+ulen)/ulen); % Number of analysis frames
n1 = 1;
n2 = alen;
Fs = 8000;
f = 0:Fs/N:Fs/2;
time = 0:1/Fs:N/Fs - 1/Fs;
S = zeros(alen/2+1, naf);
for n=1:naf % Counter over analysis frames
    
    xf = x(n1:n2); % xf = zeros(1,1024) + xf; %zero padding
    xf2 = xf.*hanning(alen);
    
    if (alen > ulen)
        X = fft(xf2);
    else
        X = fft(xf2, 512);
    end
    S(:,n) = 10*log10(abs(X(1:alen/2+1)).^2);
    n1 = n1 + ulen;
    n2 = n2 + ulen;
end

colormap(gray) % The classical spectrogram is gray,
               % type help gray for other colormaps
imagesc(time, f, flipud(-S)); % flipud flips S along the frequency axis so that
                     % frequencies increase when we move up the vertical axis
                     % -S makes black correspond to high energy!
xlabel('Time (s)')
ylabel('Frequency (Hz)')

end