function Loss = ComputeLoss(X, Y, W, b)

    P = EvaluateClassifier(X, W, b);
    for i = 1:size(Y, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    Loss = sum(Loss)/size(X, 2);

end