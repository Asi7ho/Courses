function [Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(Xt, Yt, yt, Xv, Yv, yv, GDparams, W, b, lambda)
    
    N = size(Xt, 2);
    
    n_batch = GDparams(1);
    eta = GDparams(2);
    n_epochs = GDparams(3);
    
    J_t = zeros(n_epochs, 1);
    J_v = zeros(n_epochs, 1);
    acc_t = zeros(n_epochs, 1);
    acc_v = zeros(n_epochs, 1);

    for i = 1:n_epochs
    
        for j=1:N/n_batch
            
            j_start = (j-1)*n_batch + 1;
            j_end = j*n_batch;
            inds = j_start:j_end;
            Xbatch = Xt(:, inds);
            Ybatch = Yt(:, inds);
        
            P = EvaluateClassifier(Xbatch,W,b);
        
            [grad_W, grad_b] = ComputeGradients(Xbatch,Ybatch,P,W,lambda);
            W = W - eta*grad_W;
            b = b - eta*grad_b;  
            
        end
    
        J_t(i) = ComputeCost(Xt, Yt, W, b, lambda);
        acc_t(i) = ComputeAccuracy(Xt, yt, W, b);
        
        J_v(i) = ComputeCost(Xv, Yv, W, b, lambda);
        acc_v(i) = ComputeAccuracy(Xv, yv, W, b);
    
        fprintf('On the training set epoch n°%d : Cost = %f, Acc = %f\n', i, J_t(i), acc_t(i));
        fprintf('On the validation set epoch n°%d : Cost = %f, Acc = %f\n\n', i, J_v(i), acc_v(i));
    end
    
    Wstar = W;
    bstar = b;

end