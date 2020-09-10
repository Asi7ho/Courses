%varifilter 
%performs filtering with time-varying filter coefficients.
%
%   [Y, Zf] = varifilter(B, A, X, Zi) has the same syntax as FILTER but is
%   better suited for time-varying filters. (X cannot be a matrix however.)
%
%   [Y, Zf] = varifilter(B, A, X, []) or [Y, Zf] = varifilter(B, A, X)
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
%       [Y, Z] = varifilter(B, A, X, Z); % The final filter state is returned
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
%
%   This function was previously called adaptfilt, but was changed
%   because of a name conflict with a predefined Matlab name.
%
%Arne Leijon, 2008-11-11

  function [y, Zf] = varifilter(B, A, x, Zi)

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
