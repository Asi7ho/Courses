function [grad_b, grad_W] = ComputeGradsNumSlow(X, Y, W, b, lambda, h)

no = size(W, 1);
d = size(X, 1);

grad_W = zeros(size(W));
grad_b = zeros(no, 1);

for i=1:length(b)
    b_try = b;
    b_try(i) = b_try(i) - h;
    c1 = ComputeCost(X, Y, W, b_try, lambda);
    b_try = b;
    b_try(i) = b_try(i) + h;
    c2 = ComputeCost(X, Y, W, b_try, lambda);
    grad_b(i) = (c2-c1) / (2*h);
end

for i=1:numel(W)
    
    W_try = W;
    W_try(i) = W_try(i) - h;
    c1 = ComputeCost(X, Y, W_try, b, lambda);
    
    W_try = W;
    W_try(i) = W_try(i) + h;
    c2 = ComputeCost(X, Y, W_try, b, lambda);
    
    grad_W(i) = (c2-c1) / (2*h);
end

end


function P = EvaluateClassifier(X, W, b)
    S = W*X + b;
    P = exp(S)./sum(exp(S));
end

function J = ComputeCost(X, Y, W, b, lambda)
    P = EvaluateClassifier(X, W, b);
%     for i = 1:size(Y, 2)
%         Loss(:, i) = -log(Y(:, i)'*P(:, i));
%     end
    Loss = -log(Y'*P);
    Loss = sum(diag(Loss));
    R = lambda*norm(W, 2);
    J = (Loss + R)/size(X, 1);
end

