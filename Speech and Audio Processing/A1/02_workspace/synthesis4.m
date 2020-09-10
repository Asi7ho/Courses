function s = synthesis4(E, ZC, V, A, P, ulen)

% We have included all the estimated parameters as input arguments
% but here we only use A!

n_frames = size(A,1); % Assuming filter coefficients are stored row-wise

nexc = randn(n_frames*ulen, 1);
n1 = 1;
n2 = ulen;
Z = [];
cnt = 0;
s = zeros(n_frames*ulen, 1);
for n=1:n_frames
    
    % Filter the excitation through the production (vocal tract) filter:
    if V(n) == 1
        while cnt < P(n)
            cnt = cnt + 1;
        end
        cp = P(n);
        pexc = zeros(n_frames*ulen, 1); 
        pexc(1:cp:end) = 1; 
        [s(n1:n2), Z] = varifilter(1, A(n,:), pexc(n1:n2), Z);
    else
        [s(n1:n2), Z] = varifilter(1, A(n,:), nexc(n1:n2), Z);
    end
    
    E2 = norm(s(n1:n2), 2);
    s(n1:n2) = s(n1:n2).*sqrt(E(n)/E2);
    n1 = n1+ulen;
    n2 = n2+ulen;
    cnt = 0;
end

end