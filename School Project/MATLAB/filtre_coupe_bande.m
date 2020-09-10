function filtre_coupe_bande(x, t, Fe, w0_coupe_bande, Q_cb) 

% x : signal à filtrer
% w0_coupe_bande : pulsation à rejeter
% Q_cb : Facture de qualité

y_filtre_coupe_bande(1) = 0;
y_filtre_coupe_bande(2) = 0;
for i = 3:length(t) 
   y_filtre_coupe_bande(i) = ((4*Q_cb*Fe^2 + Q_cb*w0_coupe_bande^2)*x(i) ...
                             + (2*Q_cb*w0_coupe_bande^2 - 8*Q_cb*Fe^2)*x(i-1) ...
                             + (4*Q_cb*Fe^2 + Q_cb*w0_coupe_bande^2)*x(i-2) ...
                             + (8*Q_cb*Fe^2 - 2*Q_cb*w0_coupe_bande^2)*y_filtre_coupe_bande(i-1) ...
                             + (2*Fe*w0_coupe_bande - 4*Q_cb*Fe^2 - Q_cb*w0_coupe_bande^2)*y_filtre_coupe_bande(i-2)) ...
                             /(2*Fe*w0_coupe_bande + 4*Q_cb*Fe^2 + Q_cb*w0_coupe_bande^2);
end

figure(1)
subplot(2,4,4)
plot(t, x)
axis([0 0.1 -2 2])
title('s2(t) = s1(t) + cos(2*pi*1000*t)');

subplot(2,4,8)
plot(t, y_filtre_coupe_bande)
axis([0 0.1 -2 2])
title('signal filtré coupe bande (f0 = 1000Hz, Q = 10)');

end