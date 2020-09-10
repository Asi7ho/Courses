function [A, E, d] = encoder(x, N, U, M)

n_frames = floor((length(x)-N+U)/U);

s = 1; % Index for the start of the analysis frame
e = N;% Index for the end of the analysis frame

A = zeros(n_frames,M+1);
E = zeros(n_frames,1);

x = x(:);

for n=1:n_frames
    xf = x(s:e);
    
    % Linear prediction analysis:
    [a,en] = lpc(xf.*rectwin(N),M);
    a = real(a);
    A(n,:) = a;
    E(n) = sqrt(en);
    
    d(s:e) = filter(A(n,:),1,xf)/E(n);
    
    s = s + U;
    e = e + U;
end

d = d(:);

end