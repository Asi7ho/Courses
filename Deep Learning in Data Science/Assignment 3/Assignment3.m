close all;
clear all;

addpath('Functions');
addpath('Datasets');

%% Read in / Load the data

data_fname = 'ascii_names.txt';

[X, ys, all_names] = LoadData(data_fname);
[X_train_ids, y_train_ids, X_val_ids, y_val_ids] = Load_inds(all_names, ys);

C = unique(cell2mat(all_names'));
d = numel(C);
K = length(unique(ys));
n_len = size(char(all_names), 2);

X_train = X(:, X_train_ids);
Y_train = zeros(K, length(y_train_ids));
for i=1:size(y_train_ids, 1)
    Y_train(y_train_ids(i), i) = 1;
end

X_val = X(:, X_val_ids);
Y_val = zeros(K, length(y_val_ids));
for i=1:size(y_val_ids, 1)
    Y_val(y_val_ids(i), i) = 1;
end

% X_train = X_train(:, 1:20);
% Y_train = Y_train(:, 1:20);

%% Set hyper-parameters & initialize the ConvNets parameters

n_batch = 100;
eta = 0.002;
rho = 0.9;

numberOfFilter = 2;
n(1) = 20; k(1) = 8;
n(2) = 20; k(2) = 5;
n(3) = 20; k(3) = 2;
n(4) = 20; k(4) = 2;
n(5) = 20; k(5) = 2;

for i = 1:numberOfFilter
    sizeFilters{i, 1} = [k(i), n(i)];
end

ConvNet = InitializeParameters(sizeFilters, d, n_len, K);

%% Construct the convolution matrices

F = ConvNet.F{1};
MF = MakeMFMatrix(F, n_len);

[d, k, nf] = size(ConvNet.F{1});
x_input = reshape(X_train(:, 1), [d, n_len]);
MX = MakeMXMatrix(x_input, d, k, nf);

s1 = MX * F(:);
s2 = MF * x_input(:);

fprintf('The mean square error is: %f\n', mse(s1, s2));

%% Compute Loss and accuracy

P = EvaluateFunction(X_train, ConvNet);
J = ComputeLoss(X_train, Y_train, ConvNet);
acc = ComputeAccuracy(X_train, Y_train, ConvNet);

%% Compute Gradients

[grad_F, grad_W] = ComputeGradients(X_train, Y_train, ConvNet, MX1);

%% Compare Gradient

% Gs = NumericalGradient(X_train, Y_train, ConvNet, 1e-6);
% errorF1 = abs(Gs{1} - grad_F{1}) ./ max(eps, abs(Gs{1}) + abs(grad_F{1}));
% errorF2 = abs(Gs{2} - grad_F{2}) ./ max(eps, abs(Gs{2}) + abs(grad_F{2}));
% errorW = abs(Gs{3} - grad_W) ./ max(eps, abs(Gs{3}) + abs(grad_W));
% 
% errorF1 = mean(errorF1(:));
% errorF2 = mean(errorF2(:));
% errorW = mean(errorW(:));
% 
% fprintf('Relative error of F1: %f.\n', errorF1);
% fprintf('Relative error of F2: %f.\n', errorF2);
% fprintf('Relative error of W: %f.\n', errorW);

%% Precompute MX1

[d1, k1, n1] = size(ConvNet.F{1});
for l = 1:size(X_train, 2)
    MX1{l, 1} = sparse(MakeMXMatrix(reshape(X_train(:, l), [d, n_len]), d1, k1, n1));
end

%% Mini-batch 

balanced = 1;

if balanced == 1
    n_epochs = 3000;
else
    n_epochs = 150;
end

GDparams = [n_batch, eta, rho, n_epochs];

tic
[ConvNetStar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(X_train, Y_train, ...
    X_val, Y_val, GDparams, ConvNet, balanced, MX1);
toc

figure(1)
subplot(1, 2, 1)
plot(1:n_epochs, J_t); hold on;
plot(1:n_epochs, J_v);
xlabel('epoch', 'FontSize', 18)
ylabel('Cost', 'FontSize', 18)
title('Cost function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

subplot(1, 2, 2)
plot(1:n_epochs, acc_t); hold on;
plot(1:n_epochs, acc_v);
xlabel('epoch', 'FontSize', 18)
ylabel('Accuracy', 'FontSize', 18)
title('Accuracy function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

CM_train = ConfusionMatrix(EvaluateFunction(X_train, ConvNetStar), Y_train);
CM_val = ConfusionMatrix(EvaluateFunction(X_val, ConvNetStar), Y_val);
Acc_train = 100*diag(CM_train./histcounts(y_train_ids));
Acc_val = 100*diag(CM_val./histcounts(y_val_ids));

%% Test surname

% Chau : Chinese - 2
% Beuze : German - 7
% Debain : French - 6
% Vottero : Italian - 10
% Sanchez : Spanish - 17
% Nguyen : Vietnamese - 18
% Durand : French - 6
% Li : Chinese - 2
% Ruther : German - 7
% Perez : Spanish - 17
% Fo Siong : Vietnamese - 18
% Conti : Italian - 10

surname = {'chau', 'beuze', 'debain', 'vottero', 'sanchez', 'nguyen', 'durand', ...
    'li', 'ruther', 'perez', 'fo siong', 'conti'};


for i = 1:length(surname)
    Onehot = [(surname{i} == C') zeros(d, n_len - length(surname{i}))];
    X_test(:, i) = Onehot(:);
end

P_test = EvaluateFunction(X_test, ConvNetStar);

%% sub-functions

function [X, ys, all_names] = LoadData(data_fname)
    [all_names, ys] = ExtractNames(data_fname);

    C = unique(cell2mat(all_names')); 
    d = numel(C);

    n_len = size(char(all_names), 2);

    for i = 1:length(all_names)
        Onehot = [(all_names{i} == C') zeros(d, n_len - length(all_names{i}))];
        X(:, i) = Onehot(:);
    end
end

function [X_train_ids, y_train_ids, X_val_ids, y_val_ids] = Load_inds(all_names, ys)
    X_val_ids = dlmread("Validation_Inds.txt");
    y_val_ids = ys(X_val_ids);

    X_train_ids = 1:length(all_names);
    X_train_ids(X_val_ids) = [];
    y_train_ids = ys(X_train_ids);
end

function ConvNet = InitializeParameters(sizeFilters, d, n_len, K)
    ConvNet.nlens(1) = n_len;

    sig = sqrt(2.0/(d*n_len));
    ConvNet.nlens(2) = ConvNet.nlens(1) - sizeFilters{1}(1, 1) + 1;
    ConvNet.F{1} = randn(d,  sizeFilters{1}(1, 1), sizeFilters{1}(1, 2))*sig;

    for i = 2:length(sizeFilters)
        sig = sqrt(2.0/(sizeFilters{i-1}(1, 2)*ConvNet.nlens(i)));
        ConvNet.nlens(i+1) = ConvNet.nlens(i) - sizeFilters{i}(1, 1) + 1;
        ConvNet.F{i} = randn(sizeFilters{i-1}(1, 2),  sizeFilters{i}(1, 1), sizeFilters{i}(1, 2))*sig;
    end

    fsize = (sizeFilters{end}(1, 2)*ConvNet.nlens(end));
    sig = sqrt(2.0/fsize);

    ConvNet.W = randn(K, fsize)*sig;
end

function MF = MakeMFMatrix(F, nlen)
    [dd, k, nf] = size(F);
    MF = zeros((nlen-k+1)*nf, nlen*dd);

    vectF = zeros(nf, dd*k);
    for j = 1:nf
        v = F(:, :, j);
        vectF(j, :) = v(:)';
    end
    
    for i = 0:nlen-k
       start = i*dd + 1;
       Lined = zeros(nf, nlen*dd);
       Lined(:, start:start+dd*k-1) = vectF;
       MF(i*nf+1:(i+1)*nf, :) = Lined;
    end
end

function MX = MakeMXMatrix(x_input, d, k, nf)
    nlen = size(x_input, 2);
    MX = zeros((nlen-k+1)*nf, k*nf*d);
    
    vectX = x_input(:);
    
    for i = 0:nlen-k
        start = i*d + 1;
        Lined = vectX(start:start+d*k-1)';
        MX(i*nf+1:(i+1)*nf, :) = kron(eye(nf), Lined);
    end
end

function P = EvaluateFunction(X, ConvNet)
    S{1} = ConvLayer(X, ConvNet.F{1}, ConvNet.nlens(1));
    
    for i = 2:length(ConvNet.F)
        S{i} = ConvLayer(S{i-1}, ConvNet.F{i}, ConvNet.nlens(i));
    end
    
    S = ConnectedLayer(S{end}, ConvNet.W);
    
    P = exp(S)./sum(exp(S));
end

function J = ComputeLoss(X, Y, ConvNet)
    N = size(X, 2);
    P = EvaluateFunction(X, ConvNet);
    
    J = 0;
    for i = 1:N
        J = J - log(Y(:, i)' * P(:, i));
    end
    J = J/N;
end

function acc = ComputeAccuracy(X, Y, ConvNet)
    P = EvaluateFunction(X, ConvNet);
    [~, idx] = max(P);

    y = zeros(1, size(X, 2));
    for i=1:size(X, 2)
        y(i) = find(Y(:, i));
    end
    acc = sum(y == idx)/size(X, 2);
end

function [grad_F, grad_W] = ComputeGradients(X, Y, ConvNet, MX1)
    S{1} = ConvLayer(X, ConvNet.F{1}, ConvNet.nlens(1));
    for i = 2:length(ConvNet.F)
        S{i} = ConvLayer(S{i-1}, ConvNet.F{i}, ConvNet.nlens(i));
    end
    C = ConnectedLayer(S{end}, ConvNet.W);
    P = exp(C)./sum(exp(C));
    
    g = P-Y;
    grad_W = g*S{end}'./size(X, 2);
    
   
    g = (ConvNet.W'*g).*(S{end} > 0);
    
    for i = length(ConvNet.F):-1:2
        
        grad_F{i} = zeros(size(ConvNet.F{i}));
        grad_F{i} = grad_F{i}(:);
        
        for j = 1:size(X, 2)
            gj = g(:, j);
            G = reshape(gj, [], ConvNet.nlens(i)-size(ConvNet.F{i}, 2)+1)';
            MX = sparse(MakeMXMatrix(reshape(S{i-1}(:, j), size(ConvNet.F{i}, 1), []),...
                size(ConvNet.F{i}, 1), size(ConvNet.F{i}, 2), 1));
            V = MX' * G;
            grad_F{i} = grad_F{i} + V(:)/ size(X, 2);
        end
        
        grad_F{i} = reshape(grad_F{i}, size(ConvNet.F{i}));
    
        MF = MakeMFMatrix(ConvNet.F{i}, ConvNet.nlens(i));
        g = (MF'*g).*(S{i-1} > 0);
    
    end
    
    grad_F{1} = zeros(size(ConvNet.F{1}));
    grad_F{1} = grad_F{1}(:);
    
    for j = 1:size(X, 2)
        gj = g(:, j);
        v = gj'* MX1{j};
        grad_F{1} = grad_F{1} + v'/size(X, 2);
    end
    
    grad_F{1} = reshape(grad_F{1}, size(ConvNet.F{1}));
end

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

function M = ConfusionMatrix(P, Y)
    K = size(P, 1);
    M = zeros(K, K);

    for y=1:size(Y, 2)
        ind = find(Y(:, y));
        [~, i] = max(P(:, y));
        M(ind, i) = M(ind, i) + 1;
    end
end