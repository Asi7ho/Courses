function [E, ZC, V, A, P] = analysis(x, alen, ulen, M)
% Initialization %Related to problem 7
N = length(x);
naf = floor((N-alen+ulen)/ulen); % Number of analysis frames
n1 = 1;
n2 = alen;
E = zeros(naf, 1); %Frame energy
ZC = zeros(naf, 1); %Zero crossing
V = zeros(naf, 1); % Voiced or unvoiced
A = zeros(naf, M+1); % LP coefficients with M prediction order.
P = zeros(naf, 1); %Pitch analysis

% Inside loop
for n=1:naf % Counter over analysis frames
    xf = x(n1:n2);
    E(n) = norm(xf, 2);
    nbOfZeros = 0;
    for i = 2:length(xf)
        if xf(i-1)*xf(i) < 0
            nbOfZeros = nbOfZeros + 1;
        end
    end
    ZC(n) = nbOfZeros/alen;
    if ZC(n) < 0.35
        V(n) = 1; %voiced
    else
        V(n) = 0; %unvoiced
    end
    
    % Vocal tract filter estimation using LP analysis
    c = xcorr(xf, xf, M);
    [a, ~]= levinson(c(M+1:2*M+1)); % a is a vector always starting with a 1.
    a = a(:); % Make a a column vector
    A(n,:) = a;
    
    % Pitch analysis
    c = xcorr(xf, xf);
    c = c(alen:end);
    peaks = logical([0; (c(2:end-1) > c(1:end-2)) & (c(2:end-1) > c(3:end)); 0]);
    ind = find(c == max(c(peaks)));
    P(n) = ind;
    
    n1 = n1 + ulen;
    n2 = n2 + ulen;
    
end

figure(1);clf;

subplot(3,2,1)
plot(x) % Plot the input waveform
axis([1 length(x) min(x) max(x)]);
title('waveform')

subplot(3,2,2)
plot(sqrt(E)) % Plot the standard deviation
axis([1 length(E) min(sqrt(E)) max(sqrt(E))]);
title('standard deviation')

subplot(3,2,3)
plot(V) % Plot voiced/unvoiced decision
axis([1 length(V) 0 1]);
title('voiced/unvoiced decision')

subplot(3,2,4)
plot(ZC) % Plot the normalized number of zero-crossings
axis([1 length(ZC) min(ZC) max(ZC)]);
title('normalized number of zero-crossings')

subplot(3,2,5)
F = 8000./P;
plot(F) % Plot the fundamental frequency in Hz
axis([1 length(F) 0 max(F)]);
title('fundamental frequency in Hz')

subplot(3,2,6)
S = zeros(512, naf);
for n=1:naf
    S(:,n) = 20*log10(abs(freqz(1,A(n,:),512)));
end
S = flipud(S);

colormap(gray);
imagesc(S); % Illustrate the vocal tract envelope in a spectrogram style!
title('spectrogram')

end