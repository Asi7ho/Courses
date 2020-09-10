function [x] = synthesis(E, V, A, P, U)

% [x] = synthesis(E, V, A, P, U) synthesizes speech based on the parameters
% E, P, V, and A, see help analysis for a description and returns the
% synthesized samples in x. U is the update length.
%  
% © Jonas Samuelsson, 2005

  n_frames = length(E);
  M = size(A,2) - 1;
  
  x = zeros(n_frames*U,1);
  r = zeros(n_frames*U,1);
  
  n1 = 1;
  n2 = U;
  
  pc = 0;
  Zs = [];
    
  for n=1:n_frames
        
    % Create excitation:
    % Hard decision voiced/unvoiced
    if V(n) % Voiced: Create pulse train excitation
      yf = zeros(U,1);
      for u = 1:U
	pc = pc + 1;
	if pc >= P(n)
	  yf(u) = 1;
	  pc = 0;
	end      
      end
      % Set the amplitude of the pulses such that the pulse train gets
      % the correct energy
      yf = yf * sqrt(P(n)) * E(n); % E(n) is the prediction error energy
    else % Unvoiced:
      yf = randn(U,1); % Create samples with unit variance
      yf = yf * E(n); % Energy renormalize
    end
    % end Create excitation

    % Filter excitation through production filter:
    [of, Zs] = adaptfilt(1, A(n,:), yf, Zs);
   
    
    % Insert the synthesized frame in the output vector:
    x(n1:n2) = of;
    
    n1 = n1+U;
    n2 = n2+U;
  end  

function [y, Zf] = adaptfilt(B, A, x, Zi)

%ADAPTFILT Filter with time-varying coefficients. 
%   [Y, Zf] = ADAPTFILT(B, A, X, Zi) has the same syntax as FILTER but is
%   better suited for time-varying filters. (X cannot be a matrix however.)
%
%   [Y, Zf] = ADAPTFILT(B, A, X, []) or [Y, Zf] = ADAPTFILT(B, A, X)
%   initializes the filter state with zeros.
%   
%   A typical usage could look like:
%  
%   Z = []; % Resets the filter memory.
% 
%   for i=1:n_iterations 
%       :
%       :
%       
%       [Y, Z] = adaptfilt(B, A, X, Z); % The final filter state is returned
%                                       % in Z. In the next iteration it is
%       :                               % used as initial state.
%       :
%   end  
%   
%   Zi is the initial state stored in a structure with two elements:
%   Zi.prevx and Zi.prevy which should contain past input and output samples
%   of the filter. See FILTIC for a description of how the samples should be
%   stored in Zi.
%  
%   Zf returns the final state of the filter in terms of the input and
%   output samples of the vector just filtered.
%  
%   See also FILTER, FILTIC

  nb = length(B) - 1;
  na = length(A) - 1;

  x=x(:);
  shift=length(x);
  
  % Take care of initialization if no initial state is supplied
  if nargin < 4
    Zi = [];
  end
  
  if isempty(Zi)
    Zi.prevx = zeros(nb,1);
    Zi.prevy = zeros(na,1);
  end
    
  % Calculate the filter state the way MATLAB's filter function wants it
  Z = filtic(B, A, Zi.prevy, Zi.prevx);
  
  % Filter...
  y = filter(B, A, x, Z);

  % Update the filter state
  Zf.prevx = [flipud(x);Zi.prevx(1:end-shift)];
  Zf.prevy = [flipud(y);Zi.prevy(1:end-shift)];
