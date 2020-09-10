close all;
clear all;

addpath('Functions');
addpath('Datasets');

%% Read in / Load the data

fname1 = 'condensed_2015.json'; 
val1 = jsondecode(fileread(fname1));
fname2 = 'condensed_2016.json'; 
val2 = jsondecode(fileread(fname2));
fname3 = 'condensed_2017.json'; 
val3 = jsondecode(fileread(fname3));
fname4 = 'condensed_2018.json'; 
val4 = jsondecode(fileread(fname4));

val = [val3; val4];

for i = 1:length(val)
   tweets{i, 1} = char(string(val(i).text) + "±"); 
end
book_chars = unique(cell2mat(tweets')); 

K = numel(book_chars);

n_len = size(char(tweets), 2);

start = 1;
for i = 1:length(tweets)
    X(:, start:start+length(tweets{i})-1) = (tweets{i} == book_chars');
    start = start + length(tweets{i});
end

%% Set hyper-parameters & initialize the ConvNets parameters

m = 100;
eta = 0.001;
rho = 0.9;
seqLength = 20;
n_epochs = 5;

sig = 0.01;
RNN.U = randn(m, K)*sig;
RNN.W = randn(m, m)*sig;
RNN.V = randn(K, m)*sig;
RNN.b = randn(m, 1)*0;
RNN.c = randn(K, 1)*0;

%% Mini-batch

GDparams = [seqLength, eta, rho, n_epochs];
dataset = "Trump";

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

txt_idx = SynthesizeText(RNNstar, h0, X(:, randi(size(X, 2), 1)), size(char(tweets), 2), book_chars, dataset);
txt = idxToChar(txt_idx, book_chars);
fprintf(txt);

%% 
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