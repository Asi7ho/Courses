function P = EvaluateFunction(X, ConvNet)
    
    S{1} = ConvLayer(X, ConvNet.F{1}, ConvNet.nlens(1));
    
    for i = 2:length(ConvNet.F)
        S{i} = ConvLayer(S{i-1}, ConvNet.F{i}, ConvNet.nlens(i));
    end
    
    S = ConnectedLayer(S{end}, ConvNet.W);
    
    P = exp(S)./sum(exp(S));
end