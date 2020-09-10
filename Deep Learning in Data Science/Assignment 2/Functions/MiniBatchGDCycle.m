function [Wstar, bstar, J_t, L_t, acc_t, J_v, L_v, acc_v] = MiniBatchGDCycle(Xt, Yt, yt, Xv, Yv, yv, GDparams, W, b, lambda)

    N = size(Xt, 2);

    n_batch = GDparams(1);
    eta_min = GDparams(2);
    eta_max = GDparams(3);
    n_s = GDparams(5);
    cycle = GDparams(6);
    
    J_t = zeros(2*cycle*n_s, 1);
    J_v = zeros(2*cycle*n_s, 1);
    L_t = zeros(2*cycle*n_s, 1);
    L_v = zeros(2*cycle*n_s, 1);
    acc_t = zeros(2*cycle*n_s, 1);
    acc_v = zeros(2*cycle*n_s, 1);

    j_start = 1;
    j_end = n_batch;
    l = 0;
        
    for j=1:2*cycle*n_s
        
        if j_end > N
            j_start = 1;
            j_end = n_batch;
        end
        
        if mod(j, 2*n_s) == 0
           l = l + 1; 
        end
        
        inds = j_start:j_end;
        Xbatch = Xt(:, inds);
        Ybatch = Yt(:, inds);
        
        P = EvaluateClassifier(Xbatch,W,b);
            
        if j >= 2*l*n_s && j <= (2*l + 1)*n_s
            eta = eta_min + (j-1 - 2*l*n_s)/n_s * (eta_max - eta_min);
        else
            eta = eta_max - (j-1 - (2*l+1)*n_s)/n_s * (eta_max - eta_min);
        end
        
        [grad_W, grad_b] = ComputeGradients(Xbatch,Ybatch,P,W,b,lambda);
            
        W{1} = W{1} - eta*grad_W{1};
        b{1} = b{1} - eta*grad_b{1};
        W{2} = W{2} - eta*grad_W{2};
        b{2} = b{2} - eta*grad_b{2};
            
        J_t(j) = ComputeCost(Xt, Yt, W, b, lambda);
        L_t(j) = ComputeLost(Xt, Yt, W, b);
        acc_t(j) = ComputeAccuracy(Xt, yt, W, b);
        
        J_v(j) = ComputeCost(Xv, Yv, W, b, lambda);
        L_v(j) = ComputeLost(Xv, Yv, W, b);
        acc_v(j) = ComputeAccuracy(Xv, yv, W, b);
    
        fprintf('On the training set update step n°%d : Cost = %f, Loss = %f, Acc = %f\n', j, J_t(j), L_t(j), acc_t(j));
        fprintf('On the validation set update step n°%d : Cost = %f, Loss = %f, Acc = %f\n\n', j, J_v(j), L_v(j), acc_v(j));
            
        j_start = j_start + n_batch;
        j_end = j_end + n_batch;
        
        if l == cycle
           break; 
        end
            
    end

    Wstar = W;
    bstar = b;

end