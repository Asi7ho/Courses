function t_samp = sync(mf, b_train, Q, t_start, t_end)
% t_samp = sync(mf, b_train, Q, t_start, t_end)
%
% Determines when to sample the matched filter outputs. The synchronization
% algorithm is based on cross-correlating a replica of the (known)
% transmitted training sequence with the output from the matched filter
% (before sampling). Different starting points in the matched filter output
% are tried and the shift yielding the largest value of the absolute value
% of the cross-correlation is chosen as the sampling instant for the first
% symbol.
%
% Input:
%   mf            = matched filter output, complex baseband, I+jQ
%   b_train       = the training sequence bits
%   Q             = number of samples per symbol
%   t_start       = start of search window
%   t_end         = end of search window
%
% Output:
%   t_samp = sampling instant for first symbol

QPSK_train = qpsk(b_train);
N = length(QPSK_train);
T = t_end - t_start;

for i=1:T
   cross_corr(i) = sum(conj(mf(t_start+(i-1):Q:t_start+(i-1)+Q*N-1)).*QPSK_train);
end

plot(abs(cross_corr))

[~, t_samp] = max(abs(cross_corr));
t_samp = t_start + t_samp - 1;

end







