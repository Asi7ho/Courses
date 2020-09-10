function acc = ComputeAccuracy(X, Y, ConvNet)
    P = EvaluateFunction(X, ConvNet);
    [~, idx] = max(P);

    y = zeros(1, size(X, 2));
    for i=1:size(X, 2)
        y(i) = find(Y(:, i));
    end
    acc = sum(y == idx)/size(X, 2);
end