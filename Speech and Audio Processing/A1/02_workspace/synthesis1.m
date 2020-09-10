function s = synthesis1(E, ZC, V, A, P, ulen)

% We have included all the estimated parameters as input arguments
% but here we only use A!

n_frames = size(A,1); % Assuming filter coefficients are stored row-wise

% Create a pulse train excitation:
cp = 80; % Constant pitch period in samples

pexc = zeros(n_frames*ulen, 1); 
pexc(1:cp:end) = 1;

% Create noise excitation:
nexc = randn(n_frames*ulen, 1);

n1 = 1;
n2 = ulen;
Z = [];
s = zeros(n_frames*ulen, 1);

for n=1:n_frames
    % Filter the excitation through the production (vocal tract) filter: 
    [s(n1:n2), Z] = varifilter(1, A(n,:), pexc(n1:n2), Z);
    n1 = n1+ulen;
    n2 = n2+ulen;
end

end