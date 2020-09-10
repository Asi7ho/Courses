function acc = ComputeAccuracy(X, y, W, b)
     P = EvaluateClassifier(X, W, b);
     [~, idx] = max(P);
     acc = sum(idx-1 == y(1:size(X, 2))')/size(X, 2);
end