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

%% Initializing parameters

d = size(X_train, 1);
K = size(Y_train, 1);

W = rand(K, d);
b = 0.01*randn(K, 1);

lambda = 0;
eta = 0.01;
n_batch = 100;
n_epochs = 40;

%% Evaluate Classifier

P = EvaluateClassifier(X_train, W, b);

%% Compute cost and accuracy

J = ComputeCost(X_train, Y_train, W, b, lambda);
acc = ComputeAccuracy(X_train, y_train, W, b);

%% Compute gradient

[grad_W, grad_b] = ComputeGradients(X_train, Y_train, P, W, lambda);

%% Compare Gradients

% [refGrad_b, refGrad_W] = ComputeGradsNumSlow(X_train, Y_train, W, b, lambda, 1e-5);
% 
% [max_W, max_b] = CompareGradients(grad_W, grad_b, refGrad_W, refGrad_b);


%% Train network

GDparams = [n_batch, eta, n_epochs];

[Wstar, bstar, J_t, acc_t, J_v, acc_v] = MiniBatchGD(X_train, Y_train, y_train, X_valid, Y_valid, y_valid, GDparams, W, b, lambda);

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

%% Visualize W

figure(2)
visualizeW(Wstar);


%% sub-functions

function [X, Y, y] = LoadBatch(filename)
    X = double(filename.data')/255;
    y = filename.labels;
    Y = (y == 0:max(y))';
end

function P = EvaluateClassifier(X, W, b)
    S = W*X + b;
    P = exp(S)./sum(exp(S));
end

function J = ComputeCost(X, Y, W, b, lambda)
    P = EvaluateClassifier(X, W, b);
    for i = 1:size(Y, 2)
        Loss(:, i) = -log(Y(:, i)'*P(:, i));
    end
    Loss = sum(Loss)/size(X, 2);
    R = lambda*sum(sum(W.^2));
    J = Loss + R;
end

function acc = ComputeAccuracy(X, y, W, b)
     P = EvaluateClassifier(X, W, b);
     [~, idx] = max(P);
     acc = sum(idx-1 == y(1:size(X, 2))')/size(X, 2);
end

function [grad_W, grad_b] = ComputeGradients(X, Y, P, W, lambda)
    grad_W = 2*lambda*W + (P-Y)*X'./size(X, 2);
    grad_b = sum((P-Y), 2)./size(X, 2);
end

function [max_W, max_b] = CompareGradients(grad_W, grad_b, refGrad_W, refGrad_b)

max_W = max(max(abs(grad_W - refGrad_W)));

max_b = max(abs(grad_b - refGrad_b));

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

function visualizeW(W)

    s_im = cell(10,1);
    
    for i=1:10
        im = reshape(W(i, :), 32, 32, 3);
        s_im{i} = (im - min(im(:))) / (max(im(:)) - min(im(:))); 
        s_im{i} = permute(s_im{i}, [2, 1, 3]);
    end

    montage(s_im, 'Size', [2 5]);
end
