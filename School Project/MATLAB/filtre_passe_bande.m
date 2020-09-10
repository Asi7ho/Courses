function filtre_passe_bande(x, t, Fe, w0_passe_bande, Q_pb)

% x : signal à filtrer
% w0_passe_bande : pulsation à garder
% Q_pb : Facture de qualité

y_filtre_passe_bande(1) = 0;
y_filtre_passe_bande(2) = 0;
for i = 3:length(t) 
   y_filtre_passe_bande(i) = (2*w0_passe_bande*Fe*x(i) ...
                             - 2*w0_passe_bande*Fe*x(i-2) ...
                             + (8*Q_pb*Fe^2 - 2*Q_pb*w0_passe_bande^2)*y_filtre_passe_bande(i-1) ...
                             + (2*Fe*w0_passe_bande - 4*Q_pb*Fe^2 - Q_pb*w0_passe_bande^2)*y_filtre_passe_bande(i-2)) ...
                             /(2*Fe*w0_passe_bande + 4*Q_pb*Fe^2 + Q_pb*w0_passe_bande^2);
end

figure(1)
subplot(2,4,3)
plot(t, x)
axis([0 0.1 -2 2])
title('s2(t) = s1(t) + cos(2*pi*1000*t)');

subplot(2,4,7)
plot(t, y_filtre_passe_bande)
axis([0 0.1 -2 2])
title('signal filtré passe bande (f0 = 1000Hz, Q = 10)');

end