function s_reconstruit = reconstruit_signal(s_fenetre, N)

% s_fenetre : le signal decoupe multipliee par la fenetre
% N : longueur du signal d'origine

[N_ligne,N_colonne] = size(s_fenetre);

s_reconstruit = zeros(N,1);
s_reconstruit(1:N_ligne) = s_fenetre(:, 1);

ligne = N_ligne;

for h = 2:N_colonne
    s_reconstruit(ligne/2 + 1:ligne/2 + N_ligne/2) = s_reconstruit(ligne/2 + 1:ligne/2 + N_ligne/2) + s_fenetre(1:N_ligne/2, h);
    s_reconstruit(ligne/2 + N_ligne/2 + 1:ligne/2 + N_ligne) = s_fenetre(N_ligne/2 + 1:N_ligne, h);
    ligne = ligne + N_ligne;
end

end