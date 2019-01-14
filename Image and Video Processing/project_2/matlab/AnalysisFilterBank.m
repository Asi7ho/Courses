function y = AnalysisFilterBank(x, scaling)
% x : input
% scaling : scaling argument



lScaling = scaling;
hScaling = -1.*qmf(scaling);

B_x = [x, x, x];


y0 = conv(B_x, lScaling, 'same');
y1 = conv(B_x, hScaling, 'same');

y0_correct = y0(length(x):end-length(x)-1);
y1_correct = y1(length(x):end-length(x)-1);

y0_correct = y0_correct(2:2:end);
y1_correct = y1_correct(2:2:end);

y = [y0_correct, y1_correct];

end