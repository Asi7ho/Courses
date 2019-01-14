clear all
close all

%% task 1
clear all

load('Gaussian1D.mat')

x = -20:.1:20;

figure(1)
pd1 = makedist('Normal', 'mu', mean(s1), 'sigma', var(s1));

pdf1 = pdf(pd1, x);
p1 = plot(x,pdf1,'LineWidth',2); hold on
plot([mean(s1) mean(s1)], [0 0.06368], '-');
plot([mean(s1) mean(s1)+var(s1)], [0.038208 0.038208], '-');

pd2 = makedist('Normal', 'mu', mean(s2), 'sigma', var(s2));
pdf2 = pdf(pd2, x);
p2 = plot(x,pdf2,'LineWidth',2); hold on
plot([mean(s2) mean(s2)], [0 0.1819], '-');
plot([mean(s2) mean(s2)+var(s2)], [0.10914 0.10914], '-');

pd3 = makedist('Normal', 'mu', mean(s3), 'sigma', var(s3));
pdf3 = pdf(pd3, x);
p3 = plot(x,pdf3,'LineWidth',2); hold on
plot([mean(s3) mean(s3)], [0 0.2033], '-');
plot([mean(s3) mean(s3)+var(s3)], [0.12198 0.12198], '-');

title('')
legend([p1 p2 p3], {'pdf(x_1(n)) : E[x_1(n)] = 1.3829 / Var[x_1(n)] = 6.2650', 'pdf(x_2(n)) : E[x_2(n)] = 0.6135 / Var[x_2(n)] = 2.1931', 'pdf(x_3(n)) : E[x_3(n)] = 0.4408 / Var[x_3(n)] = 1.9615'}, 'Fontsize', 18);

%% task 2
clear all

load('Gaussian2D.mat')

figure(2)
subplot(1,4,1)
mu = mean(s1);
Sigma = corrcoef(s1);
x1 = -3:.2:3; y1 = -3:.2:3;
[X1,Y1] = meshgrid(x1,y1);
F1 = mvnpdf([X1(:) Y1(:)],mu,Sigma);
F1 = reshape(F1,length(y1),length(x1));
surf(x1,y1,F1);
caxis([min(F1(:))-.5*range(F1(:)),max(F1(:))]);
axis([-3 3 -3 3 0 .2])
xlabel('x_1'); ylabel('y_1'); zlabel('Probability Density');

subplot(1,4,3)
mu = mean(s2);
Sigma = corrcoef(s2);
x2 = -3:.2:3; y2 = -3:.2:3;
[X2,Y2] = meshgrid(x2,y2);
F2 = mvnpdf([X2(:) Y2(:)],mu,Sigma);
F2 = reshape(F2,length(y2),length(x2));
surf(x2,y2,F2);
caxis([min(F2(:))-.5*range(F2(:)),max(F2(:))]);
axis([-3 3 -3 3 0 .3])
xlabel('x_2'); ylabel('y_2'); zlabel('Probability Density');

subplot(1,4,2)
contour(x1,y1,F1);
xlabel('x_1'); ylabel('y_1');
title('joint pdf f_{XY}^1(x, y) see from above', 'FontSize', 14);

subplot(1,4,4)
contour(x2,y2,F2);
xlabel('x_2'); ylabel('y_2');
title('joint pdf f_{XY}^2(x, y) see from above', 'FontSize', 14);

%% Task 4
clear all

load('SinusInNoise1.mat');

figure(4)
subplot(1,2,1)
periodogram(y1);
title('Power spectrum of output 1');
subplot(1,2,2)
periodogram(y2);
title('Power spectrum of output 2');

%% Task 5
clear all

load('SinusInNoise2.mat');

figure(4)
periodogram(y);
title('Periodogram');

%% Task6

clear all

f = 0:0.01:0.5;

R1 = 1./(17/16 - 0.5*cos(2*pi*f));
R2 = 1./(17/16 - 0.5*cos(2*pi*f)).^2;

subplot(1,2,1)
plot(f, R1);
xlabel('Normalized Frequency'); ylabel('Power');
title('R_{x_1}(\nu)')
axis([0 0.5 0 3.5])
subplot(1,2,2)
plot(f, R2);
xlabel('Normalized Frequency'); ylabel('Power');
title('R_{x_2}(\nu)')
axis([0 0.5 0 3.5])

%% Task 7
clear all

N = 50;

k = -N:0.01:N;

r = 0.25.^abs(k);
c = 256/225 * conv(r, r);
plot(-2*N:0.01:2*N, c);
title('r_{x2}(k)')
xlabel('k'); ylabel('convolution');

x=256/225*(0.25.^abs(k).*(abs(k).*(1+0.25^2) + 1- 0.25^2))/(1 - 0.25^2);

