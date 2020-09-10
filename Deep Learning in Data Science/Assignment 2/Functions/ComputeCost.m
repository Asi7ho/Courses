function J = ComputeCost(X, Y, W, b, lambda)
    P = EvaluateClassifier(X, W, b);
    for i = 1:size(Y, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    Loss = sum(Loss)/size(X, 2);
    R = lambda*(sum(sum(W{1}.^2)) + sum(sum(W{2}.^2)));
    J = Loss + R;
end