function J = ComputeLoss(RNN, h, X, Y)

    P = EvaluateFunction(RNN, h, X);
    for i = 1:size(X, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    J = sum(Loss)/size(X, 2);

end