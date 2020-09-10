%{ 
    Script pour faire un debruitage de note par decomposition parcimonieuse
    
    On va utiliser differents types de dictionnaire pour comparer
    l'efficacite du debruitage
    
    On commence d'abord par les dictionnaires unitaires et on va, petit a
    petit, vers un dictionnaire aleatoire où une methode de descente d'optimisation sera
    necessaire

    On utilise dans ce code la methode d'optimisation par descente du
    gradient. 

    On peut ameliorer ce programme en utilisant soit un critere d'arret
    pour la methode du gradient pour eviter de faire des boucles inutiles
    soit utiliser la methode de Newton, ce qui permet d'accelerer la
    resolution du probleme  
%}

clear all
close all
clc

% Type de décomposition 
typeDecomposition = 'unitaire_cos';
	% unitaire_cos
	% unitaire_rand
    % cos_non_unitaire
    % cos_sin_non_unitaire
	% random_dico
 
% Ouverture du fichier
nameFile = './signaux/13_16kHz.mat';
load(nameFile);

N = length(x);
t = 0:1/fs:(N-1)/fs;

% Affichage des courbes d'origines
figure(1);
subplot(3,1,1);
plot(t, s);
xlabel('temps (s)');
ylabel('signal mV');
title('Signal originel');
        
subplot(3,1,2);
plot(t, x);
xlabel('temps (s)');
ylabel('signal mV');
title('Signal bruite');

switch typeDecomposition
    
    case 'unitaire_cos' % On fait une decomposition sur un dictionnaire de cosinus discret 
        
        % Declaration des parametres
        lambda = 0.55;
        N_decoupage = 4000;
        
        % Creation du dictionnaire
        D = create_dico(N_decoupage, typeDecomposition);
        
        % Creation de la fenetre d'analyse
        triangle = window(@triang, N_decoupage);
        
        % Creation de la matrice du signal decoupe
        [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, triangle, lambda, typeDecomposition);
        
    case 'unitaire_rand' % On fait une decomposition sur un dictionnaire unitaire aleatoire
        
        % Declaration des parametres
        lambda = 0.25;
        N_decoupage = 2000;

        % Creation du dictionnaire
        D = create_dico(N_decoupage, typeDecomposition);
        
        % Creation de la fenetre d'analyse
        triangle = window(@triang, N_decoupage);
        
        % Creation de la matrice du signal decoupe
        [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, triangle, lambda, typeDecomposition);
        
    case 'cos_non_unitaire'  % On fait une decomposition sur un dictionnaire non unitaire à l'aide d'un dictionnaire de cos discret
        
        % Declaration des parametres
        lambda = 0.2;
        N_decoupage = 500;
        
        % Creation du dictionnaire
        D = create_dico(N_decoupage, typeDecomposition);
        
        % Creation de la fenetre d'analyse
        triangle = window(@triang, N_decoupage);
        
        % Creation de la matrice du signal decoupe
        [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, triangle, lambda, typeDecomposition);
        
    case 'cos_sin_non_unitaire' % On fait une decomposition sur un dictionnaire non unitaire à l'aide d'un dictionnaire de cos et sin discrets
        % Declaration des parametres
        lambda = 0.40;
        N_decoupage = 500;
        
        % Creation du dictionnaire
        D = create_dico(N_decoupage, typeDecomposition);
        
        % Creation de la fenetre d'analyse
        triangle = window(@triang, N_decoupage);
        
        % Creation de la matrice du signal decoupe
        [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, triangle, lambda, typeDecomposition);
        
    case 'random_dico' % On fait une decomposition sur un dictionnaire aleatoire
        % Declaration des parametres
        lambda = 0;
        N_decoupage = 500;
        
        % Creation du dictionnaire
        D = create_dico(N_decoupage, typeDecomposition);
        
        % Creation de la fenetre d'analyse
        triangle = window(@triang, N_decoupage);
        
        % Creation de la matrice du signal decoupe
        [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, triangle, lambda, typeDecomposition);
        
end

% On reconstitue le signal
s_reconstruit = reconstruit_signal(s_fenetre, N);
        
% Affichage des resultats et du taux d'erreur
figure(1);
subplot(3,1,3);
plot(t, s_reconstruit);
xlabel('temps (s)');
ylabel('signal mV');
title('Signal reconstruit [ lambda = 0.4 ]');

err = norm(s - s_reconstruit, 2)^2/N;
t_parc = sum(taux)*100/(2*N);
disp(['EQM =  ' num2str(err)]);
disp(['le taux de parcimonie est de : ' num2str(t_parc) '%']);