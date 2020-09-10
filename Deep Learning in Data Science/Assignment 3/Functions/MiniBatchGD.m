function [ConvNetStar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(Xt, Yt, Xv, Yv, GDparams, ConvNet, balanced, MX1)
    best_acc = 0;

    n_batch = GDparams(1);
    eta = GDparams(2);
    rho = GDparams(3);
    n_epochs = GDparams(4);
    
    J_t = zeros(n_epochs, 1);
    J_v = zeros(n_epochs, 1);
    acc_t = zeros(n_epochs, 1);
    acc_v = zeros(n_epochs, 1);
    
    for l = 1:length(ConvNet.F)
        v{l} = zeros(size(ConvNet.F{l}));
    end
    vw = zeros(size(ConvNet.W));

    for i = 1:n_epochs
        
        if balanced == 1
            [Xt2, Yt2, MXt] = BalancedData(Xt, Yt, MX1);
        else
            perm = randperm(size(Xt, 2), size(Xt, 2));
            Xt2 = Xt(:, perm); 
            Yt2 = Yt(:, perm);
            MXt = MX1(perm);
        end
       
        N = size(Xt2, 2);
    
        for j=1:floor(N/n_batch)
            
            j_start = (j-1)*n_batch + 1;
            j_end = j*n_batch;
            inds = j_start:j_end;
            Xbatch = Xt2(:, inds);
            Ybatch = Yt2(:, inds);
        
            [grad_F, grad_W] = ComputeGradients(Xbatch, Ybatch, ConvNet, MXt);
            
            for l = 1:length(ConvNet.F)
                v{l} = rho.*v{l} + eta.*grad_F{l};
                ConvNet.F{l} = ConvNet.F{l} - v{l}; 
            end
            vw = rho.*vw + eta.*grad_W;
            ConvNet.W = ConvNet.W - vw;
            
        end
    
        J_t(i) = ComputeLoss(Xt, Yt, ConvNet);
        acc_t(i) = ComputeAccuracy(Xt, Yt, ConvNet);
        
        J_v(i) = ComputeLoss(Xv, Yv, ConvNet);
        acc_v(i) = ComputeAccuracy(Xv, Yv, ConvNet);
    
        fprintf('On the training set epoch n°%d : Loss = %f, Acc = %f\n', i, J_t(i), acc_t(i));
        fprintf('On the validation set epoch n°%d : Loss = %f, Acc = %f\n\n', i, J_v(i), acc_v(i));
        
        if best_acc < acc_v(i)
           best_acc = acc_v;
           ConvNetStar = ConvNet;
        end
        
    end
end