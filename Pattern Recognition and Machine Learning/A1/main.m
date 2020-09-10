close all;
clear all;

addpath('PattRecClasses-2')

%% Initialize parameters
q = [0.75, 0.25];
A = [0.99, 0.01; 0.03, 0.97];

%% Markov Chain
T = 10000;

mc = MarkovChain(q, A);

S = rand(mc, T);

fS1 = sum(S == 1)/T;
fS2 = sum(S == 2)/T;

% P(St = 1) = 0.75
% P(St = 2) = 0.25

%% HMM

muu1 = 0;
muu2 = 3;

sigma1 = 1;
sigma2 = 2;

b1 = GaussD('Mean', muu1, 'StDev', sigma1);
b2 = GaussD('Mean', muu2, 'StDev', sigma2);

B = [b1, b2];

h = HMM(mc, B);

[X, ~] = rand(h, T);

m_X = mean(X);
var_X = var(X);

true_m_X = q(1)*muu1 + q(2)*muu2;
true_var_X = q(1)*sigma1^2 + q(2)*sigma2^2 + q(1)*(muu1 - true_m_X)^2 + q(2)*(muu2 - true_m_X)^2;

%% Randomness HMM - 1

T = 500;

h = HMM(mc, B);

[X, Seq1] = rand(h, T);

figure(1)
plot(1:T, X); hold on;
plot(1:T, Seq1, '.');
title('Output of the HMM source', 'FontSize', 18);
xlabel('Time Step (t)', 'FontSize', 18)
ylabel('HMM source output (X_t)', 'FontSize', 18)
legend({'HMM output', 'Corresponding Source'}, 'FontSize', 18)

%% Randomness HMM - 2

b1 = GaussD('Mean', 0, 'StDev', 1);
b2 = GaussD('Mean', 0, 'StDev', 5);
B = [b1, b2];

T = 500;

h = HMM(mc, B);

[X, Seq2] = rand(h, T);

figure(2)
plot(1:T, X); hold on;
plot(1:T, Seq2, '.');
title('Output of the HMM source', 'FontSize', 18);
xlabel('Time Step (t)', 'FontSize', 18)
ylabel('HMM source output (X_t)', 'FontSize', 18)
legend({'HMM output', 'Corresponding Source'}, 'FontSize', 18)

%% Finite HMM

q = [0.75, 0.25];
A = [0.99, 0.01, 0; 0.03, 0.95, 0.02];

b1 = GaussD('Mean', 0, 'StDev', 1);
b2 = GaussD('Mean', 3, 'StDev', 2);

B = [b1, b2];

T = 1000;

mc = MarkovChain(q, A);

h = HMM(mc, B);

[X, Seq3] = rand(h, T);

figure(3)
plot(X); hold on;
plot(Seq3, '.');
title('Output of the HMM source', 'FontSize', 18);
xlabel('Time Step (t)', 'FontSize', 18)
ylabel('HMM source output (X_t)', 'FontSize', 18)
legend({'HMM output', 'Corresponding Source'}, 'FontSize', 18)
axis([0 T min(X)-1 max(X)+1])

%% HMM with Gaussian vector distributions

q = [0.75, 0.25];
A = [0.99, 0.01; 0.03, 0.95];

muu1 = [-3; -1];
muu2 = [3; 1];

Sigma1 = [1 0; 0 1];
Sigma2 = [2 1; 1 4];

b1 = GaussD('Mean', muu1, 'Covariance', Sigma1);
b2 = GaussD('Mean', muu2, 'Covariance', Sigma2);

B = [b1, b2];

T = 10000;

mc = MarkovChain(q, A);

h = HMM(mc, B);

[X, Seq4] = rand(h, T);

M_X = mean(X, 2);
Cov_X = cov(X(1, :), X(2, :));

true_M_X = q(1)*muu1 + q(2)*muu2;
true_Cov_X = q(1)*Sigma1 + q(2)*Sigma2 + ...
    q(1)*(muu1 - true_M_X)*(muu1 - true_M_X)' + q(2)*(muu2 - true_M_X)*(muu2 - true_M_X)';

figure(4)
plot(X(1, :), X(2, :), '+'); hold on;
title('Output of the HMM source', 'FontSize', 18);
xlabel('HMM source output X_1(t)', 'FontSize', 18)
ylabel('HMM source output X_2(t)', 'FontSize', 18)
