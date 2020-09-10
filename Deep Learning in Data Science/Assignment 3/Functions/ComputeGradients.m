function [grad_F, grad_W] = ComputeGradients(X, Y, ConvNet, MX1)

    S{1} = ConvLayer(X, ConvNet.F{1}, ConvNet.nlens(1));
    for i = 2:length(ConvNet.F)
        S{i} = ConvLayer(S{i-1}, ConvNet.F{i}, ConvNet.nlens(i));
    end
    C = ConnectedLayer(S{end}, ConvNet.W);
    P = exp(C)./sum(exp(C));
    
    g = P-Y;
    grad_W = g*S{end}'./size(X, 2);
    
   
    g = (ConvNet.W'*g).*(S{end} > 0);
    
    for i = length(ConvNet.F):-1:2
        
        grad_F{i} = zeros(size(ConvNet.F{i}));
        grad_F{i} = grad_F{i}(:);
        
        for j = 1:size(X, 2)
            gj = g(:, j);
            G = reshape(gj, [], ConvNet.nlens(i)-size(ConvNet.F{i}, 2)+1)';
            MX = sparse(MakeMXMatrix(reshape(S{i-1}(:, j), size(ConvNet.F{i}, 1), []),...
                size(ConvNet.F{i}, 1), size(ConvNet.F{i}, 2), 1));
            V = MX' * G;
            grad_F{i} = grad_F{i} + V(:)/ size(X, 2);
        end
        
        grad_F{i} = reshape(grad_F{i}, size(ConvNet.F{i}));
    
        MF = MakeMFMatrix(ConvNet.F{i}, ConvNet.nlens(i));
        g = (MF'*g).*(S{i-1} > 0);
    
    end
    
    grad_F{1} = zeros(size(ConvNet.F{1}));
    grad_F{1} = grad_F{1}(:);
    
    for j = 1:size(X, 2)
        gj = g(:, j);
        v = gj'* MX1{j};
        grad_F{1} = grad_F{1} + v'/size(X, 2);
    end
    
    grad_F{1} = reshape(grad_F{1}, size(ConvNet.F{1}));
end