clear all 
close all

addpath('functions');
addpath('Datasets/cifar-10-batches-mat');

%% Train, Valid and Test Sets 

A_train = load('data_batch_1.mat');
A_valid = load('data_batch_2.mat');
A_test = load('test_batch.mat');

[X_train, Y_train, y_train] = LoadBatch(A_train);
[X_valid, Y_valid, y_valid] = LoadBatch(A_valid);
[X_test, Y_test, y_test] = LoadBatch(A_test);

% size_valid = 5000;
% 
% A_train1 = load('data_batch_1.mat');
% A_train2 = load('data_batch_2.mat');
% A_train3 = load('data_batch_3.mat');
% A_train4 = load('data_batch_4.mat');
% A_train5 = load('data_batch_5.mat');
% A_test = load('test_batch.mat');
% 
% [X_train1, Y_train1, y_train1] = LoadBatch(A_train1);
% [X_train2, Y_train2, y_train2] = LoadBatch(A_train2);
% [X_train3, Y_train3, y_train3] = LoadBatch(A_train3);
% [X_train4, Y_train4, y_train4] = LoadBatch(A_train4);
% [X_train5, Y_train5, y_train5] = LoadBatch(A_train5);
% 
% X_train = [X_train1, X_train2, X_train3, X_train4, X_train5(:, 1:end-size_valid)];
% Y_train = [Y_train1, Y_train2, Y_train3, Y_train4, Y_train5(:, 1:end-size_valid)];
% y_train = [y_train1', y_train2', y_train3', y_train4', y_train5(1:end-size_valid)']';
% 
% X_valid = X_train5(:, end-size_valid+1:end);
% Y_valid = Y_train5(:, end-size_valid+1:end);
% y_valid = y_train5(end-size_valid+1:end);
% 
% [X_test, Y_test, y_test] = LoadBatch(A_test);

mean_X = mean(X_train, 2);
std_X = std(X_train, 0, 2);

X_train = X_train - repmat(mean_X, [1, size(X_train, 2)]);
X_train = X_train ./ repmat(std_X, [1, size(X_train, 2)]);

X_valid = X_valid - repmat(mean_X, [1, size(X_valid, 2)]);
X_valid = X_valid ./ repmat(std_X, [1, size(X_valid, 2)]);

X_test = X_test - repmat(mean_X, [1, size(X_test, 2)]);
X_test = X_test ./ repmat(std_X, [1, size(X_test, 2)]);

%% Initializing parameters

lambda = 0.01;
eta = 3e-3;
n_batch = 100;
n_epochs = 200;

eta_min = 1e-5;
eta_max = 1e-1;
n_s = 2*floor(size(X_train, 2)/n_batch);
cycle = 2;

d = size(X_train, 1);
K = size(Y_train, 1);

nbrOfHiddenLayer = 1; % More hidden layers/nodes (b)
m = 50*ones(nbrOfHiddenLayer, 1);

[W, b] = InitializeParameters(K, m, d, nbrOfHiddenLayer);

%% k hidden layers

GDparams = [n_batch, eta, n_epochs];

[Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(X_train, Y_train, y_train, ...
    X_valid, Y_valid, y_valid, GDparams, W, b, lambda, nbrOfHiddenLayer);

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

J_t = ComputeCost(X_test, Y_test, Wstar, bstar, lambda, nbrOfHiddenLayer);
acc_t = ComputeAccuracy(X_test, y_test, Wstar, bstar, nbrOfHiddenLayer);
fprintf('\nOn the test set : Loss = %f, Acc = %f\n\n\n', J_t, acc_t);

%% k hidden layers cycle

eta_min = 0.001;
eta_max = 0.007;
n_s = 12*floor(size(X_train, 2)/n_batch);
lambda = 0.0014;
cycle = 3;

% for k = 1:15
%     %cycle = k;
%     
%     n_s = k*floor(size(X_train, 2)/n_batch);
%     
%     GDparams = [n_batch, eta_min, eta_max, n_epochs, n_s, cycle];
%     
%     [Wstar, bstar, ~, ~, ~, ~] = MiniBatchGDCycle(X_train, Y_train, y_train,...
%         X_valid, Y_valid, y_valid, GDparams, W, b, lambda, nbrOfHiddenLayer);
%     
%     Acc(k) = ComputeAccuracy(X_test, y_test, Wstar, bstar, nbrOfHiddenLayer);
% end
% 
% plot(2:8, Acc(2:8));
% xlabel('number of cycle', 'FontSize', 18)
% ylabel('Test Accuracy', 'FontSize', 18)
% title('Test Accuracy vs number of cycle', 'FontSize', 18)

GDparams = [n_batch, eta_min, eta_max, n_epochs, n_s, cycle];

[Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGDCycle(X_train, Y_train, y_train,...
         X_valid, Y_valid, y_valid, GDparams, W, b, lambda, nbrOfHiddenLayer);

figure(2)
subplot(1, 2, 1)
plot(1:2*cycle*n_s, J_t); hold on;
plot(1:2*cycle*n_s, J_v);
xlabel('update step', 'FontSize', 18)
ylabel('Loss', 'FontSize', 18)
title('Loss function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)

subplot(1, 2, 2)
plot(1:2*cycle*n_s, acc_t); hold on;
plot(1:2*cycle*n_s, acc_v);
xlabel('update step', 'FontSize', 18)
ylabel('Accuracy', 'FontSize', 18)
title('Accuracy function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)

J_te = ComputeCost(X_test, Y_test, Wstar, bstar, lambda, nbrOfHiddenLayer);
acc_te = ComputeAccuracy(X_test, y_test, Wstar, bstar, nbrOfHiddenLayer);
fprintf('\nOn the test set : Loss = %f, Acc = %f\n\n\n', J_te, acc_te);

%% Find eta_min and eta_max

eta_min = 0;
eta_max = 2e-2;
eta = eta_min:1e-3:eta_max;

lambda = 0;
n_epochs = 8;

for i = 1:length(eta)

GDparams = [n_batch, eta(i), n_epochs];

[Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(X_train, Y_train, y_train, ...
    X_valid, Y_valid, y_valid, GDparams, W, b, lambda, nbrOfHiddenLayer);

Acc(i) = ComputeAccuracy(X_test, y_test, Wstar, bstar, nbrOfHiddenLayer);

end

figure(3)
plot(eta, Acc);
xlabel('learning rate', 'FontSize', 18)
ylabel('Accuracy', 'FontSize', 18)
title('Accuracy vs learning rate', 'FontSize', 18)

% eta_min = 0.001
% eta_max = 0.007

%% sub-functions

function [X, Y, y] = LoadBatch(filename)
    X = double(filename.data')./255;
    y = filename.labels;
    Y = (y == 0:max(y))';
end

function [W, b] = InitializeParameters(K, m, d, nbrOfHiddenLayer)
    Wi = 1/sqrt(d)*randn(m(1), d);
    bi = 0*randn(m(1), 1);
    W{1} = Wi;
    b{1} = bi;
    
    for i = 2:nbrOfHiddenLayer
        Wi = 1/sqrt(m(i-1))*randn(m(i), m(i-1));
        bi = 0*randn(m(i), 1);
        W{i} = Wi;
        b{i} = bi;
    end

    Wi = 1/sqrt(m(end))*randn(K, m(end));
    bi = 0*randn(K, 1);
    
    W{nbrOfHiddenLayer+1} = Wi;
    b{nbrOfHiddenLayer+1} = bi;
end

function P = EvaluateClassifier(X, W, b, nbrOfHiddenLayer)
    S = W{1}*X + b{1};
    H = max(0, S);
    
    for i = 2:nbrOfHiddenLayer
        S = W{i}*H + b{i};
        H = max(0, S);
    end
    
    S = W{nbrOfHiddenLayer+1}*H + b{nbrOfHiddenLayer+1};
    P = exp(S)./sum(exp(S));
end

function J = ComputeCost(X, Y, W, b, lambda, nbrOfHiddenLayer)
    P = EvaluateClassifier(X, W, b, nbrOfHiddenLayer);
    for i = 1:size(Y, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    Loss = sum(Loss)/size(X, 2);
    
    S = 0;
    for i = 1:nbrOfHiddenLayer+1
        S = S + sum(sum(W{i}.^2));
    end
    
    R = lambda*S;
    J = Loss + R;
end

function acc = ComputeAccuracy(X, y, W, b, nbrOfHiddenLayer)
     P = EvaluateClassifier(X, W, b, nbrOfHiddenLayer);
     [~, idx] = max(P);
     acc = sum(idx-1 == y(1:size(X, 2))')/size(X, 2);
end

function [grad_W, grad_b] = ComputeGradients(X, Y, P, W, b, lambda, nbrOfHiddenLayer)
    
    S{1} = W{1}*X + b{1};
    %S{1} = S{1}.*(rand(size(S{1})) < 0.8); %Dropout (d)
    H{1} = max(0, S{1});
    
    for i = 2:nbrOfHiddenLayer
        S{i} = W{i}*H{i-1} + b{i};
        %S{i} = S{i}.*(rand(size(S{i})) < 0.5);
        H{i} = max(0, S{i});
    end

    g{nbrOfHiddenLayer+1} = P-Y;
    
    for i = nbrOfHiddenLayer:-1:1
        g{i} = (g{i+1}'*W{i+1})'.*(S{i} > 0);
    end
    
    for i = nbrOfHiddenLayer+1:-1:1
        grad_b{i} = sum(g{i}, 2)./size(X, 2);
    end

    for i = nbrOfHiddenLayer+1:-1:2
        grad_W{i} = g{i}*H{i-1}'./size(X, 2) + 2*lambda.*W{i};
    end
    
    grad_W{1} = g{1}*X'./size(X, 2) + 2*lambda.*W{1};
end

function [Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(Xt, Yt, yt, Xv, Yv, yv, GDparams, W, b, lambda, nbrOfHiddenLayer)
    
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
        
            P = EvaluateClassifier(Xbatch,W,b,nbrOfHiddenLayer);
        
            [grad_W, grad_b] = ComputeGradients(Xbatch,Ybatch,P,W,b,lambda,nbrOfHiddenLayer);
            
            for k = 1:nbrOfHiddenLayer+1
                W{k} = W{k} - eta*grad_W{k};
                b{k} = b{k} - eta*grad_b{k};
            end
            
        end
    
        J_t(i) = ComputeCost(Xt, Yt, W, b, lambda, nbrOfHiddenLayer);
        acc_t(i) = ComputeAccuracy(Xt, yt, W, b, nbrOfHiddenLayer);
        
        J_v(i) = ComputeCost(Xv, Yv, W, b, lambda, nbrOfHiddenLayer);
        acc_v(i) = ComputeAccuracy(Xv, yv, W, b, nbrOfHiddenLayer);
    
        fprintf('On the training set epoch n°%d : Loss = %f, Acc = %f\n', i, J_t(i), acc_t(i));
        fprintf('On the validation set epoch n°%d : Loss = %f, Acc = %f\n\n', i, J_v(i), acc_v(i));
    end
    
    Wstar = W;
    bstar = b;

end

function [Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGDCycle(Xt, Yt, yt, Xv, Yv, yv, GDparams, W, b, lambda, nbrOfHiddenLayer)

    N = size(Xt, 2);

    n_batch = GDparams(1);
    eta_min = GDparams(2);
    eta_max = GDparams(3);
    n_epochs = GDparams(4);
    n_s = GDparams(5);
    cycle = GDparams(6);
    
    J_t = zeros(n_epochs, 1);
    J_v = zeros(n_epochs, 1);
    acc_t = zeros(n_epochs, 1);
    acc_v = zeros(n_epochs, 1);

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
        
        P = EvaluateClassifier(Xbatch,W,b,nbrOfHiddenLayer);
            
        if j >= 2*l*n_s && j <= (2*l + 1)*n_s
            eta = eta_min + (j-1 - 2*l*n_s)/n_s * (eta_max - eta_min);
        else
            eta = eta_max - (j-1 - (2*l+1)*n_s)/n_s * (eta_max - eta_min);
        end
        
        [grad_W, grad_b] = ComputeGradients(Xbatch,Ybatch,P,W,b,lambda,nbrOfHiddenLayer);
            
        for k = 1:nbrOfHiddenLayer+1
            W{k} = W{k} - eta*grad_W{k};
            b{k} = b{k} - eta*grad_b{k};
        end
            
        J_t(j) = ComputeCost(Xt, Yt, W, b, lambda,nbrOfHiddenLayer);
        acc_t(j) = ComputeAccuracy(Xt, yt, W, b,nbrOfHiddenLayer);
        
        J_v(j) = ComputeCost(Xv, Yv, W, b, lambda,nbrOfHiddenLayer);
        acc_v(j) = ComputeAccuracy(Xv, yv, W, b,nbrOfHiddenLayer);
    
        fprintf('On the training set update step n°%d : Loss = %f, Acc = %f\n', j, J_t(j), acc_t(j));
        fprintf('On the validation set update step n°%d : Loss = %f, Acc = %f\n\n', j, J_v(j), acc_v(j));
            
        j_start = j_start + n_batch;
        j_end = j_end + n_batch;
        
        if l == cycle
           break; 
        end
            
    end

    Wstar = W;
    bstar = b;

end