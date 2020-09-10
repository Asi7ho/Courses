function filtre_passe_haut(x, t, Fe, wc_passe_haut)

% x : signal à filtrer
% wc_passe_haut : pulsation de coupure

y_filtre_passe_haut(1) = 0;
y_filtre_passe_haut(2) = 0;
for i = 3:length(t) 
    y_filtre_passe_haut(i) = (4*Fe^2*x(i) ...
                           - 8*Fe^2*x(i-1) ...
                           + 4*Fe^2*x(i-2) ...
                           - (4*Fe^2 - 4*Fe*wc_passe_haut + wc_passe_haut^2)*y_filtre_passe_haut(i-2) ...
                           - (-8*Fe^2 + 2*wc_passe_haut^2)*y_filtre_passe_haut(i-1)) ...
                           / (4*Fe^2 + 4*Fe*wc_passe_haut + wc_passe_haut^2);
                       
end

figure(1)
subplot(2,4,2)
plot(t, x)
axis([0 0.1 -2 2])
title('s1(t) = cos(2*pi*100*t) + cos(2*pi*5000*t)');

subplot(2,4,6)
plot(t, y_filtre_passe_haut)
axis([0 0.1 -1 1])
title('signal filtré passe haut (fc = 100Hz)');

end