function [RNNstar, J, txts] = MiniBatchAdaGrad(Xt, GDparams, RNN, book_chars, dataset)

    if dataset == "Trump"
        n = 330;
    else
        n = 200;
    end
    
    tidx = 1;
    minLoss = inf;
    n_batch = GDparams(1);
    eta = GDparams(2);
    rho = GDparams(3);
    n_epochs = GDparams(4);
    
    M.U = zeros(size(RNN.U));
    M.V = zeros(size(RNN.V));
    M.W = zeros(size(RNN.W));
    M.b = zeros(size(RNN.b));
    M.c = zeros(size(RNN.c));
    
    smoothLoss = 0;
    step = 0;
    
    for i = 1:n_epochs
        
        e = 1;
        h = zeros(size(RNN.W, 1), 1);
        
        while e < size(Xt, 2) - n_batch - 1
            
            Xbatch = Xt(:, e:e+n_batch-1);
            Ybatch = Xt(:, e+1:e+n_batch);
            
            if dataset == "Trump"
                for c = 1:size(Xbatch, 2)
                    if idxToChar(Xbatch(:, c), book_chars) == '±'
                        h = zeros(size(RNN.W, 1), 1);
                        break;
                    end
                end
            end
            
            [A, P, H, Loss] = ForwardPass(RNN, Xbatch, Ybatch, h);
            
            grads = ComputeGradient(RNN, Xbatch, Ybatch, A, H, P);
            
            for f = fieldnames(RNN)'
                M.(f{1}) = rho.*M.(f{1}) + (1-rho).*(grads.(f{1})).^2;
                RNN.(f{1}) = RNN.(f{1}) - eta*grads.(f{1})./sqrt(M.(f{1}) + eps);
            end
            
            if step == 0
                smoothLoss = Loss;
            else
                smoothLoss = 0.999*smoothLoss + 0.001*Loss;
            end
            
            h = H(:, end);
            e = e + n_batch;
            step = step + 1;
            if step == 1 || mod(step, 100) == 0
                J(step) = smoothLoss;
                fprintf('Update step n°%d : smooth loss = %f\n', step, smoothLoss);
            end
            
            if mod(step, 500) == 0
                txt_idx = SynthesizeText(RNN, h, Xt(:, randi(size(Xt, 2), 1)), n, book_chars, dataset);
                txts{tidx, 1} = idxToChar(txt_idx, book_chars);
                tidx = tidx + 1;
            end

            if smoothLoss < minLoss
                minLoss = smoothLoss;
                RNNstar = RNN;
            end
            
        end
        
    end
end