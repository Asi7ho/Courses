function P = EvaluateClassifier(X, W, b)
    S1 = W{1}*X + b{1};
    H = max(0, S1);
    S = W{2}*H + b{2};
    P = exp(S)./sum(exp(S));
end