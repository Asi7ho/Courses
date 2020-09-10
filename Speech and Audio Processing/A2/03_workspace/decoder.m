function [xhat] = decoder(A, E, d, U)
s = 1;
e = U;

n_frames = size(A, 1);

for n=1:n_frames
    df = d(s:e)*E(n);
    
    xhat(s:e) = filter(1, A(n,:), df);
    
    s = s + U;
    e = e + U;
end

xhat = xhat(:);
end