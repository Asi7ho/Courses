function [W, b] = InitializeParameters(K, m, d)
    W1 = 1/sqrt(d)*randn(m, d);
    b1 = 0*randn(m, 1);

    W2 = 1/sqrt(m)*randn(K, m);
    b2 = 0*randn(K, 1);

    W = {W1, W2};
    b = {b1, b2};
end