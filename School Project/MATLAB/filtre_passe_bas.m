function filtre_passe_bas(x, t, Fe, wc_passe_bas)

% x : signal à filtrer
% wc_passe_bas : pulsation de coupure

y_filtre_passe_bas(1) = 0;
y_filtre_passe_bas(2) = 0;

for i = 3:length(t) 
   y_filtre_passe_bas(i) = (x(i)*wc_passe_bas^2 ...
                            + x(i-1)*2*wc_passe_bas ...
                            + x(i-2)*wc_passe_bas^2 ...
                            - (4*Fe^2 - 4*Fe*wc_passe_bas + wc_passe_bas^2)*y_filtre_passe_bas(i-2) ...
                            - (-8*Fe^2 + 2*wc_passe_bas^2)*y_filtre_passe_bas(i-1)) ...
                            / (4*Fe^2 + 4*wc_passe_bas*Fe +wc_passe_bas^2);
                         
end

figure(1)
subplot(2,4,1)
plot(t, x)
axis([0 0.1 -2 2])
title('s1(t) = cos(2*pi*100*t) + cos(2*pi*5000*t)');

subplot(2,4,5)
plot(t, y_filtre_passe_bas)
axis([0 0.1 -1 1])
title('signal filtré passe bas (fc = 100 Hz)');


end