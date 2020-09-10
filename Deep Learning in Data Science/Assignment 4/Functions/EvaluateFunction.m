function P = EvaluateFunction(RNN, h, X)

for i = 1:size(X, 2)
    a = RNN.U*X(:, i) + RNN.W*h + RNN.b;
    h = tanh(a);
    o = RNN.V*h + RNN.c;
    P(:, i) = exp(o)./sum(exp(o));
end

end