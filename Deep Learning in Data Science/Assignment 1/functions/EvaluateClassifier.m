function P = EvaluateClassifier(X, W, b)
    S = W*X + b;
    P = exp(S)./sum(exp(S));
end