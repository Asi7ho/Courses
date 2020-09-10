close all;
clear all;

addpath('PattRecClasses-2')

%% Test forward - Finite MC

q = [1; 0];
A = [0.9, 0.1, 0; 0, 0.9, 0.1];

b1 = GaussD('Mean', 0, 'StDev', 1);
b2 = GaussD('Mean', 3, 'StDev', 2);

B = [b1; b2];

mc = MarkovChain(q, A);

x = [-0.2, 2.6, 1.3];
pX = prob(B, x);

[alfaHat_f, c_f] = forward(mc, pX);

%% Test forward - Infinite MC

q = [1; 0; 0];
A = [0.3, 0.7, 0; 0, 0.5, 0.5; 0, 0, 1];

b1 = DiscreteD([1, 0, 0, 0]);
b2 = DiscreteD([0, 0.5, 0.4, 0.1]);
b3 = DiscreteD([0.1, 0.1, 0.2, 0.6]);

B = [b1; b2; b3];

mc = MarkovChain(q, A);

x = [1, 2, 4, 4, 1];
pX = prob(B, x);

[alfaHat_i, c_i] = forward(mc, pX);

%% Test logP

h = HMM(mc, B);
logP = logprob(h, x);