function phihat = phase_estimation(r, b_train)
% phihat = phase_estimation(r, b_train)
%
% Phase estimator using the training sequence. The phase estimate is
% obtained by minimizing the norm of the difference between the known
% transmitted QPSK-modulated training sequence and the received training
% part. NB! There are other ways of estimating the phase, this is just
% one example.
%
% Input:
%   r       = received baseband signal
%   b_train = the training sequence bits
%
% Output:
%   phihat     = estimated phase

QPSK_train = qpsk(b_train);
N = length(QPSK_train);
r_train = r(1:N);

min = inf;

for phi = 0:0.01:2*pi
    r_phi = r_train*exp(-1i*phi);
    rau_phi = norm(r_phi - QPSK_train);
    if rau_phi < min
       min = rau_phi;
       phihat = phi;
    end
end

end

