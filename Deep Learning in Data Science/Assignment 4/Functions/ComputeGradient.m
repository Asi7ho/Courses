function grads = ComputeGradient(RNN, X, Y, A, H, P)
    
    N = size(X, 2);

    grads.o = P - Y;
    grads.V = grads.o*H(:, 2:end)'./N;
    grads.c = sum(grads.o, 2)./N;
   
    grads.H(:, N) = grads.o(:, N)'*RNN.V;  
    grads.a(:, N) = grads.H(:, N)'*diag(1 - tanh(A(:, size(X, 2))).^2);

    for i = N-1:-1:1
        grads.H(:, i) = grads.o(:, i)'*RNN.V + grads.a(:, i+1)'*RNN.W;
        grads.a(:, i) = grads.H(:, i)'*diag(1 - tanh(A(:, i)).^2);
    end
    
    grads.b = sum(grads.a, 2)./N;
    grads.W = grads.a*H(:, 1:end-1)'./N;
    grads.U = grads.a*X'./N;
    
    for f = fieldnames(grads)'
       grads.(f{1}) = max(min(grads.(f{1}), 5), -5);
    end

end