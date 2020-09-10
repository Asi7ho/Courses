function [grad_W, grad_b] = ComputeGradients(X, Y, P, W, lambda)
    grad_W = 2*lambda*W + (P-Y)*X'./size(X, 2);
    grad_b = sum((P-Y), 2)./size(X, 2);
end