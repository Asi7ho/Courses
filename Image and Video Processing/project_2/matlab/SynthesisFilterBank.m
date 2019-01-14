function x = SynthesisFilterBank(y, scaling)
% x : input
% scaling : scaling argument

lScaling = fliplr(scaling);
hScaling = fliplr(-1.*qmf(scaling));

cA = y(1:length(y)/2);
cD = y(length(y)/2+1:end);

cA = upsample(cA,2);
cD = upsample(cD,2);

cA_x = [cA, cA, cA];
cD_x = [cD, cD, cD];

x0 = conv(cA_x, lScaling, 'same');
x1 = conv(cD_x, hScaling, 'same');

x0_correct = x0(length(cA):end-length(cA)-1);
x1_correct = x1(length(cD):end-length(cD)-1);

x = x0_correct + x1_correct;

end