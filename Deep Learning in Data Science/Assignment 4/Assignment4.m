close all;
clear all;

addpath('Functions');
addpath('Datasets');

%% Read in / Load the data

book_fname = 'goblet_book.txt'; 
fid = fopen(book_fname, 'r'); 
book_data = fscanf(fid,'%c'); 
fclose(fid);

book_chars = unique(book_data);
K = numel(book_chars);

n_len = length(book_data);

for i = 1:n_len
    X(:, i) = (book_data(i) == book_chars)';
end

X0 = X(:, 1:20);
Y0 = X(:, 2:21);

%% Set hyper-parameters & initialize the ConvNets parameters

m = 5;
eta = 0.001;
rho = 0.9;
seqLength = 20;
n_epochs = 10;

sig = 0.01;
RNN.U = randn(m, K)*sig;
RNN.W = randn(m, m)*sig;
RNN.V = randn(K, m)*sig;
RNN.b = randn(m, 1)*0;
RNN.c = randn(K, 1)*0;

%% Synthesize text from your randomly initialized RNN

n = 200;

h0 = randn(m, 1)*0;

txt_idx = SynthesizeText(RNN, h0, X(:, randi(size(X, 2), 1)), n, book_chars, "Goblet");
txt = idxToChar(txt_idx, book_chars);
fprintf(txt);

%% Forward Pass

[A, P, H, Loss] = ForwardPass(RNN, X0, Y0, h0);

%% Backward Pass

grads = ComputeGradient(RNN, X0, Y0, A, H, P);

%% Compare gradients

num_grads = ComputeGradsNum(X0, Y0, RNN, 1e-6);

errorU = abs(num_grads.U - grads.U) ./ max(eps, abs(num_grads.U) + abs(grads.U));
errorV = abs(num_grads.V - grads.V) ./ max(eps, abs(num_grads.V) + abs(grads.V));
errorW = abs(num_grads.W - grads.W) ./ max(eps, abs(num_grads.W) + abs(grads.W));
errorb = abs(num_grads.b - grads.b) ./ max(eps, abs(num_grads.b) + abs(grads.b));
errorc = abs(num_grads.c - grads.c) ./ max(eps, abs(num_grads.c) + abs(grads.c));

errorU = mean(errorU(:));
errorV = mean(errorV(:));
errorW = mean(errorW(:));
errorb = mean(errorb(:));
errorc = mean(errorc(:));

fprintf('Relative error of U: %f.\n', errorU);
fprintf('Relative error of V: %f.\n', errorV);
fprintf('Relative error of W: %f.\n', errorW);
fprintf('Relative error of b: %f.\n', errorb);
fprintf('Relative error of c: %f.\n', errorc);

%% Mini-batch

GDparams = [seqLength, eta, rho, n_epochs];
dataset = "Goblet";

[RNNstar, J, txts] = MiniBatchAdaGrad(X, GDparams, RNN, book_chars, dataset);

us = 0:100:length(J);
us(1) = 1;
figure(1)
plot(us, J(us))
title('Smooth loss RNN', 'FontSize', 18);
xlabel('Update Step', 'FontSize', 18);
ylabel('Smooth Loss', 'FontSize', 18);

%% Test

h0 = randn(m, 1)*sig;

txt_idx = SynthesizeText(RNNstar, h0, X(:, randi(length(book_data), 1)), 200, book_chars, dataset);
txt = idxToChar(txt_idx, book_chars);
fprintf(txt);

%% sub-functions

function idx = charToIdx(char, book_chars)
    idx = (char == book_chars)';
end

function txt = idxToChar(X, book_chars)
    for i = 1:size(X, 2)
       txt(i) = book_chars(find(X(:, i)));
    end
end

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

function J = ComputeLoss(RNN, h, X, Y)

    P = EvaluateFunction(RNN, h, X);
    for i = 1:size(X, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    J = sum(Loss)/size(X, 2);

end

function P = EvaluateFunction(RNN, h, X)

for i = 1:size(X, 2)
    a = RNN.U*X(:, i) + RNN.W*h + RNN.b;
    h = tanh(a);
    o = RNN.V*h + RNN.c;
    P(:, i) = exp(o)./sum(exp(o));
end

end

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

function Y = SynthesizeText(RNN, h0, x0, n, book_chars, dataset)

    h = h0;
    x = x0;
    
    K = size(RNN.U, 2);
    
    Y(:, 1) = x;

    for i = 2:n
       a = RNN.U*x + RNN.W*h + RNN.b;
       h = tanh(a);
       o = RNN.V*h + RNN.c;
       p = exp(o)./sum(exp(o));
      
       cp = cumsum(p);
       a = rand;
       ixs = find(cp-a > 0);
       ii = ixs(1); 

       Y(:, i) = zeros(K, 1);
       Y(ii, i) = 1;
       x = Y(:, i);
       
       if dataset == "Trump"
           if idxToChar(x, book_chars) == '±'
               break;
           end
       end 
    end
end
