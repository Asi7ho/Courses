function J = ComputeLoss(X, Y, ConvNet)
    N = size(X, 2);
    P = EvaluateFunction(X, ConvNet);
    
    J = 0;
    for i = 1:N
        J = J - log(Y(:, i)' * P(:, i));
    end
    J = J/N;
end