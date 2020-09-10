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
        
            [grad_W, grad_b] = ComputeGradients(Xbatch,Ybatch,P,W,b,lambda);
            W{1} = W{1} - eta*grad_W{1};
            b{1} = b{1} - eta*grad_b{1};
            W{2} = W{2} - eta*grad_W{2};
            b{2} = b{2} - eta*grad_b{2};
            
        end
    
        J_t(i) = ComputeCost(Xt, Yt, W, b, lambda);
        acc_t(i) = ComputeAccuracy(Xt, yt, W, b);
        
        J_v(i) = ComputeCost(Xv, Yv, W, b, lambda);
        acc_v(i) = ComputeAccuracy(Xv, yv, W, b);
    
        fprintf('On the training set epoch n°%d : Loss = %f, Acc = %f\n', i, J_t(i), acc_t(i));
        fprintf('On the validation set epoch n°%d : Loss = %f, Acc = %f\n\n', i, J_v(i), acc_v(i));
    end
    
    Wstar = W;
    bstar = b;

end