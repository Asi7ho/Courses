function ConvNet = InitializeParameters(sizeFilters, d, n_len, K)

ConvNet.nlens(1) = n_len;

sig = sqrt(2.0/(d*n_len));
ConvNet.nlens(2) = ConvNet.nlens(1) - sizeFilters{1}(1, 1) + 1;
ConvNet.F{1} = randn(d,  sizeFilters{1}(1, 1), sizeFilters{1}(1, 2))*sig;

for i = 2:length(sizeFilters)
    sig = sqrt(2.0/(sizeFilters{i-1}(1, 2)*ConvNet.nlens(i)));
    ConvNet.nlens(i+1) = ConvNet.nlens(i) - sizeFilters{i}(1, 1) + 1;
    ConvNet.F{i} = randn(sizeFilters{i-1}(1, 2),  sizeFilters{i}(1, 1), sizeFilters{i}(1, 2))*sig;
end

fsize = (sizeFilters{end}(1, 2)*ConvNet.nlens(end));
sig = sqrt(2.0/fsize);

ConvNet.W = randn(K, fsize)*sig;

end