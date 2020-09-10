clear all 
close all

addpath('Functions');
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
eta = 1e-2;
n_batch = 100;
n_epochs = 200;

eta_min = 1e-5;
eta_max = 1e-1;
n_s = 3*floor(size(X_train, 2)/n_batch);
cycle = 3;

d = size(X_train, 1);
K = size(Y_train, 1);
m = 50;

[W, b] = InitializeParameters(K, m, d);

%% Evaluate Classifier

P = EvaluateClassifier(X_train, W, b);

%% Compute cost and accuracy

J = ComputeCost(X_train, Y_train, W, b, lambda);
acc = ComputeAccuracy(X_train, y_train, W, b);

%% Compute gradient

[grad_W, grad_b] = ComputeGradients(X_train, Y_train, P, W, b, lambda);

%% Compare Gradients

% [refGrad_b, refGrad_W] = ComputeGradsNumSlow(X_train, Y_train, W, b, lambda, 1e-6);
% 
% [max_W, max_b] = CompareGradients(grad_W, grad_b, refGrad_W, refGrad_b);

%% Train network

GDparams = [n_batch, eta, n_epochs];

[Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(X_train, Y_train, y_train, ...
    X_valid, Y_valid, y_valid, GDparams, W, b, lambda);

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

%% Test

J_t = ComputeCost(X_test, Y_test, Wstar, bstar, lambda);
acc_t = ComputeAccuracy(X_test, y_test, Wstar, bstar);
fprintf('\nOn the test set : Cost = %f, Acc = %f\n\n\n', J_t, acc_t);




%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%% cyclical learning rates %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Train network with cyclical learning rates

GDparams = [n_batch, eta_min, eta_max, n_epochs, n_s, cycle];

[Wstar, bstar, J_t, L_t, acc_t, J_v, L_v, acc_v] = MiniBatchGDCycle(X_train, Y_train, y_train,...
    X_valid, Y_valid, y_valid, GDparams, W, b, lambda);

figure(2)
subplot(1, 3, 1)
plot(1:2*cycle*n_s, J_t); hold on;
plot(1:2*cycle*n_s, J_v);
xlabel('update step', 'FontSize', 18)
ylabel('Cost', 'FontSize', 18)
title('Cost function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

subplot(1, 3, 2)
plot(1:2*cycle*n_s, L_t); hold on;
plot(1:2*cycle*n_s, L_v);
xlabel('update step', 'FontSize', 18)
ylabel('Loss', 'FontSize', 18)
title('Loss function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

subplot(1, 3, 3)
plot(1:2*cycle*n_s, acc_t); hold on;
plot(1:2*cycle*n_s, acc_v);
xlabel('update step', 'FontSize', 18)
ylabel('Accuracy', 'FontSize', 18)
title('Accuracy function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

%% Test cyclical learning rates

J_t = ComputeCost(X_test, Y_test, Wstar, bstar, lambda);
L_t = ComputeLost(X_test, Y_test, Wstar, bstar);
acc_t = ComputeAccuracy(X_test, y_test, Wstar, bstar);
fprintf('\nOn the test set : Cost = %f, Loss = %f, Acc = %f\n\n\n', J_t, L_t, acc_t);

%% %%%%%%%%%%%%%%%%%%%% Coarse-to-fine random search to set lambda. %%%%%%%%%%%%%%%%%%%%%%% 
%% Find best lambda

% GDparams = [n_batch, eta_min, eta_max, n_epochs, n_s, cycle];
% 
% for i = 1:20
% 
% disp(i);
%     
% lmin = -5;
% lmax = -1;
% 
% l = lmin + (lmax - lmin)*rand(1, 1); 
% lambda(i) = 10^l;
% 
% 
% 
% [~, ~, ~, ~, ~, ~, ~, acc_v] = MiniBatchGDCycle(X_train, Y_train, y_train,...
%     X_valid, Y_valid, y_valid, GDparams, W, b, lambda(i));
% 
% Acc(i) = max(acc_v);
% 
% end
% 
% figure(3)
% stem(lambda, Acc);
% xlabel('\lambda', 'FontSize', 18)
% ylabel('Max Accuracy', 'FontSize', 18)
% title('Coarse-to-fine random search to set \lambda', 'FontSize', 18)

lambda = 5.4e-4;
n_batch = 100;
eta_min = 0.001;
eta_max = 0.007;
n_s = 3*floor(size(X_train, 2)/n_batch);
cycle = 3;

GDparams = [n_batch, eta_min, eta_max, n_epochs, n_s, cycle];

[Wstar, bstar, J_t, L_t, acc_t, J_v, L_v, acc_v] = MiniBatchGDCycle(X_train, Y_train, y_train,...
     X_valid, Y_valid, y_valid, GDparams, W, b, lambda);
 
figure(4)
subplot(1, 3, 1)
plot(1:2*cycle*n_s, J_t); hold on;
plot(1:2*cycle*n_s, J_v);
xlabel('update step', 'FontSize', 18)
ylabel('Cost', 'FontSize', 18)
title('Cost function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

subplot(1, 3, 2)
plot(1:2*cycle*n_s, L_t); hold on;
plot(1:2*cycle*n_s, L_v);
xlabel('update step', 'FontSize', 18)
ylabel('Loss', 'FontSize', 18)
title('Loss function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

subplot(1, 3, 3)
plot(1:2*cycle*n_s, acc_t); hold on;
plot(1:2*cycle*n_s, acc_v);
xlabel('update step', 'FontSize', 18)
ylabel('Accuracy', 'FontSize', 18)
title('Accuracy function', 'FontSize', 18)
legend({'Training set', 'Validation Set'}, 'FontSize', 18)
axis square

J_t = ComputeCost(X_test, Y_test, Wstar, bstar, lambda);
L_t = ComputeLost(X_test, Y_test, Wstar, bstar);
acc_t = ComputeAccuracy(X_test, y_test, Wstar, bstar);
fprintf('\nOn the test set : Cost = %f, Loss = %f, Acc = %f\n\n\n', J_t, L_t, acc_t);

%% sub-functions

function [X, Y, y] = LoadBatch(filename)
    X = double(filename.data')./255;
    y = filename.labels;
    Y = (y == 0:max(y))';
end

function [W, b] = InitializeParameters(K, m, d)
    W1 = 1/sqrt(d)*randn(m, d);
    b1 = 0*randn(m, 1);

    W2 = 1/sqrt(m)*randn(K, m);
    b2 = 0*randn(K, 1);

    W = {W1, W2};
    b = {b1, b2};
end

function P = EvaluateClassifier(X, W, b)
    S1 = W{1}*X + b{1};
    H = max(0, S1);
    S = W{2}*H + b{2};
    P = exp(S)./sum(exp(S));
end

function Loss = ComputeLoss(X, Y, W, b)

    P = EvaluateClassifier(X, W, b);
    for i = 1:size(Y, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    Loss = sum(Loss)/size(X, 2);

end

function J = ComputeCost(X, Y, W, b, lambda)
    Loss = ComputeLoss(X, Y, W, b);
    R = lambda*(sum(sum(W{1}.^2)) + sum(sum(W{2}.^2)));
    J = Loss + R;
end

function acc = ComputeAccuracy(X, y, W, b)
     P = EvaluateClassifier(X, W, b);
     [~, idx] = max(P);
     acc = sum(idx-1 == y(1:size(X, 2))')/size(X, 2);
end

function [grad_W, grad_b] = ComputeGradients(X, Y, P, W, b, lambda)
    
    S1 = W{1}*X + b{1};
    H = max(0, S1);

    g2 = P-Y;
    g1 = (g2'*W{2})'.*(S1 > 0);
    
    grad_b2 = sum(g2, 2)./size(X, 2);
    grad_b1 = sum(g1, 2)./size(X, 2);
    
    grad_W2 = g2*H'./size(X, 2) + 2*lambda.*W{2};
    grad_W1 = g1*X'./size(X, 2) + 2*lambda.*W{1};
    
    grad_W = {grad_W1, grad_W2};
    grad_b = {grad_b1, grad_b2};
end

function [max_W, max_b] = CompareGradients(grad_W, grad_b, refGrad_W, refGrad_b)

    for i = 1:length(grad_W)
        max_W(i) = max(max(abs(grad_W{i} - refGrad_W{i})));
    end

    for i = 1:length(grad_b)
        max_b(i) = max(max(abs(grad_b{i} - refGrad_b{i})));
    end

end

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