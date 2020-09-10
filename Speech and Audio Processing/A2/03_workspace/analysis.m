function [E, V, A, P] = analysis(x, N, U, M)

% [E, V, A, P] = analysis(x, N, U, M) extracts vocoder parameters E, V, A,
% and P from the speech signal x on a frame by frame basis. N is the
% analysis frame length, U is the update length, and M is the order of the
% linear prediction analysis. The return arguments are the extracted
% parameters, where E, V, and P are vectors with "n_frames" (the number of
% analysis frames, see code below) elements, A is a (n_frames, M+1) size
% matrix (includes the leading 1 of the polynomial).
% 
% E contains the square root of the prediction error energy for each
% analysis frame, V is the voiced/unvoiced decision (1 if voiced, 0 if
% unvoiced), A contains the LP parameters (row-wise) for each frame, and P
% contains the pitch period for each frame.
%
% Note that the pitch estimator uses a frame length that is set inside this
% function (the variable Np). All other estimators are based on N samples.
% 
% © Jonas Samuelsson, 2005
  
  n_frames = floor((length(x)-N+U)/U);

  s = 1; % Index for the start of the analysis frame
  e = N;% Index for the end of the analysis frame

  pmem = 15*ones(5,1); % Memory of the pitch smoothing median-like filter
  [blp,alp] = cheby1(5,0.5,2*700/8000); % Low-pass filter for pitch estimation
  Zlp = []; % The filter state of the pitch estimation lowpass filter
  
  E = zeros(n_frames,1);
  P = zeros(n_frames,1);
  V = zeros(n_frames,1);
  ZC = zeros(n_frames,1);
  A = zeros(n_frames,M+1);

  % Make x a column vector
  x = x(:);
  
  % Fix for the pitch estimator which always uses Np samples
  Np = 400;
  diff = Np-N;
  xp = [zeros(diff/2,1); x; zeros(diff/2+U+1,1)];

  for n=1:n_frames
    xf = x(s:e);

    % Linear prediction analysis:
    [a,en] = lpc(xf.*hanning(N),M);
    a = real(a);    
    A(n,:) = a;
    E(n) = sqrt(en); % Return the prediction error energy
    % end lp analysis
    
    % Calculate zero crossings
    zc1 = xf(2:end).*xf(1:end-1);
    zc = length(find(zc1<0));
    zc = zc / N;
    
    ZC(n) = zc;
    
    if zc > 60/160
      V(n) = 0; % unvoiced
    else
      V(n) = 1;
    end
    % end zero-crossings 
    
    % The pitch estimator is very sensitive to varying frame length.
    % Here we always use Np samples in our estimate:
    center = floor((s+e)/2 + diff/2+1); % Center of the frame in the xp vector...
    xpf = xp(center-Np/2:center+Np/2-1);

    % Low-pass filter before pitch estimation, cut off 700 Hz:
    y = filter(blp, alp, xpf, Zlp);
    [dummy,Zlp] = filter(blp, alp, xpf(1:U), Zlp); % Set the memory of the filter correctly
        
    % Pitch smoothing without extra delay using median-like filtering
    p = max([pitch1(y) 15]); 
    if(abs(8000*(1/p-1/pmem(end))) > 100) & (abs(8000*(1/p-1/pmem(end-1))) > 50) & (abs(8000*(1/p-1/pmem(end-2))) > 30)& (abs(8000*(1/p-1/pmem(end-3))) > 10),
      ptemp = (p+pmem(2))/2;
      p = pmem(2);
      pmem = [pmem(2:end); ptemp];
    else 
      pmem = [pmem(2:end); p];
    end
    P(n) = p;    

    % end pitch estimation
    
    s = s + U;
    e = e + U;
  end
    
%   figure(1);clf;
%   subplot(3,2,1)
%   plot(x)
%   axis([1 length(x) min(x) max(x)]);
% 
%   subplot(3,2,2)
%   plot(E)
%   axis([1 length(E) min(E) max(E)]);
%   
%   subplot(3,2,3)
%   plot(V, '.')
%   axis([1 length(V) min(V) max(V)]);
% 
%   subplot(3,2,4)
%   plot(ZC)
%   axis([1 length(ZC) min(ZC) max(ZC)]);
% 
%   subplot(3,2,5)
%   F = 8000./P;
%   plot(F)
%   axis([1 length(F) 0 600]);
%   
%   subplot(3,2,6)
%   S = zeros(512, n_frames);
%   for n=1:n_frames
%     S(:,n) = 20*log10(abs(freqz(1,A(n,:),512)));
%   end
%   S = flipud(S);
%   colormap(gray);
%   imagesc(S);  
    
  
function p = pitch1(in)

%
  N = length(in);
  low_th = 20; % The lowest pitch period we can estimate (=400 Hz at Fs=8Khz)
       
  in = in.*hann(N);
  c = xcorr(in);
  c = c(N+1+low_th:2*N-1);
  [dummy, idx] = max(c);
  
  p = idx + low_th;
  
function idx = pitch2(in)
  
%  idx = pitch(in) estimates the fundamental frequency of the signal using
%  cepstral processing. "in" is a vector of speech samples (or the
%  LP-residual; prediction error sequence). "idx" is the pitch period (in
%  samples).
 
  framelen = length(in);
  fftlen = 4096;
  L = 20; % Window length when extracting the vocal tract part of the cepstrum
  
  % Centerclip the frame
  ma = max(abs(in));
  idx = find(abs(in) < 0.5*ma);
  in(idx) = 0;
  
  X = fft(in, fftlen);
  C = log(abs(X)); % The real cepstrum
  c = real(ifft(C,fftlen)); % c is guaranteed to be real since C is symmetric
  p = [zeros(L,1); c(L+1:end-L); zeros(L,1)];
  p = p(1:framelen);

  % Find pitch period
  [m,idx] = max(p);
  if idx > framelen 
    idx = framelen;
  end

