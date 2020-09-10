function y = lowpass(x, fc, N)

% Y = LOWPASS(X, FC) filters the vector x with a low-pass filter with
% cut-off frequency FC, 0 < FC <0.5. The filter is linear phase, designed
% using the window method. The filter length is 1024.
%  
% Y = LOWPASS(X, FC, N) allows to set the filter length N.   
  
  if nargin == 2
    N=1024;  
  end

  b = fir1(N, 2*fc);
  y = filter(b, 1, x);