function [A, P, H, Loss] = ForwardPass(RNN, X, Y, h0)

    h = h0;

    A = zeros(size(h0, 1), size(X, 2));
    H = zeros(size(h0, 1), size(X, 2) + 1);
    P = zeros(size(Y, 1), size(X, 2));
    
    H(:, 1) = h;
    Loss = 0;
    
    for i = 1:size(X, 2)
        a = RNN.U*X(:, i) + RNN.W*h + RNN.b;
        h = tanh(a);
        o = RNN.V*h + RNN.c;
        P(:, i) = exp(o)./sum(exp(o));
        
        A(:, i) = a;
        H(:, i+1) = h;

        Loss = Loss - log(Y(:, i)'*P(:, i));
    end
    
end