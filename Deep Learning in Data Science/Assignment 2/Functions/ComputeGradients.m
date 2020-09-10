function [grad_W, grad_b] = ComputeGradients(X, Y, P, W, b, lambda)
    
    S1 = W{1}*X + b{1};
    H = max(0, S1);

    g2 = P-Y;
    g1 = (g2'*W{2})'.*(S1 > 0);
    
    grad_b2 = sum(g2, 2)./size(X, 2);
    grad_b1 = sum(g1, 2)./size(X, 2);
    
    grad_W2 = g2*H'./size(X, 2) + 2*lambda.*W{2};
    grad_W1 = g1*X'./size(X, 2) + 2*lambda.*W{1};
    
    grad_W = {grad_W1, grad_W2};
    grad_b = {grad_b1, grad_b2};
end