function [s_fenetre, taux] = decoupe_signal(x, D, N_decoupage, fenetre, lambda, typeDecomposition)

% x : signal a debuiter
% D : dictionnaire de decomposition
% N_decoupage : longueur de decoupage de signal a debruiter
% fenetre : la fenetre avec laquelle on doit multiplier le signal
% lambda : parametre d'optimisation
% typeDecomposition : permet de choisir la decoupe du signal

switch typeDecomposition
    
    case 'unitaire_cos'
        debut = 1;
        fin = N_decoupage;
        N = length(x);

        for k = 1:2*N/N_decoupage -2
            X = x(debut:fin, 1);
            Y = D'*X;
    
            for i = 1:N_decoupage
                if Y(i, 1) > lambda
                    alpha(i, 1) = Y(i, 1) - lambda;
                elseif Y(i) < -lambda
                    alpha(i, 1) = Y(i, 1) + lambda;
                else 
                    alpha(i, 1) = 0;
                end
            end
            
            s_decoupe(:, k) = D*alpha;
            taux(k, 1) = nnz(alpha);
    
            debut = debut + N_decoupage/2;
            fin = fin + N_decoupage/2;
        end

        [N_ligne,N_colonne] = size(s_decoupe);

        for l = 1:N_colonne
            s_fenetre(:, l) = s_decoupe(:, l).*fenetre;
        end
        
    case 'unitaire_rand'
        debut = 1;
        fin = N_decoupage;
        N = length(x);

        for k = 1:2*N/N_decoupage -2
            X = x(debut:fin, 1);
            Y = D'*X;
    
            for i = 1:N_decoupage
                if Y(i, 1) > lambda
                    alpha(i, 1) = Y(i, 1) - lambda;
                elseif Y(i) < -lambda
                    alpha(i, 1) = Y(i, 1) + lambda;
                else 
                    alpha(i, 1) = 0;
                end
            end
            
            s_decoupe(:, k) = D*alpha;
            taux(k, 1) = nnz(alpha);
    
            debut = debut + N_decoupage/2;
            fin = fin + N_decoupage/2;
        end

        [N_ligne,N_colonne] = size(s_decoupe);

        for l = 1:N_colonne
            s_fenetre(:, l) = s_decoupe(:, l).*fenetre;
        end
        
    case 'cos_non_unitaire' % On utilise la methode de la descente du gradient
        debut = 1;
        fin = N_decoupage;
        N = length(x);
        rho = 5E-3;
        nbItMax = 1000;
        
        for k = 1:2*N/N_decoupage -2
            X = x(debut:fin, 1);

            % a. Initialisation
            alpha = zeros(N_decoupage, 1);		% valeur initiale des parametres
            J(1) = (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1); % Valeur du critere pour les parametres initiaux
	
            % b. Iterations
            for ind = 2:nbItMax
			
                % Calcul du gradient
                gradJ = -2*D'*(X - D*alpha) + lambda*sign(alpha);
			
                % mise a jour des parametres
                alpha = alpha - rho*gradJ;
                J(ind) =  (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1);
            end
    
            s_decoupe(:, k) = D*alpha;
            taux(k, 1) = nnz(alpha);
    
            debut = debut + N_decoupage/2;
            fin = fin + N_decoupage/2;
        end
        
        %{
        figure(2);
        plot((1:nbItMax), J(1:nbItMax));
        title('Evolution du critère');
        xlabel('nombre iteration');
        ylabel('critère');
        %}
        
        [N_ligne,N_colonne] = size(s_decoupe);

        for l = 1:N_colonne
            s_fenetre(:, l) = s_decoupe(:, l).*fenetre;
        end

    case 'cos_sin_non_unitaire'
        debut = 1;
        fin = N_decoupage;
        N = length(x);
        rho = 5E-3;
        nbItMax = 300;
        
        for k = 1:2*N/N_decoupage -2
            X = x(debut:fin, 1);

            % a. Initialisation
            alpha = zeros(N_decoupage, 1);		% valeur initiale des parametres
            J(1) = (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1); % Valeur du critere pour les parametres initiaux
	
            % b. Iterations
            for ind = 2:nbItMax
			
                % Calcul du gradient
                gradJ = -2*D'*(X - D*alpha) + lambda*sign(alpha);
			
                % mise a jour des parametres
                alpha = alpha - rho*gradJ;
                J(ind) =  (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1);
            end
    
            s_decoupe(:, k) = D*alpha;
            taux(k, 1) = nnz(alpha);
    
            debut = debut + N_decoupage/2;
            fin = fin + N_decoupage/2;
        end

        %{
        figure(2);
        plot((1:nbItMax), J(1:nbItMax));
        title('Evolution du critère');
        xlabel('nombre iteration');
        ylabel('critère');
        %}
        
        [N_ligne,N_colonne] = size(s_decoupe);

        for l = 1:N_colonne
            s_fenetre(:, l) = s_decoupe(:, l).*fenetre;
        end
        
    case 'random_dico'
        debut = 1;
        fin = N_decoupage;
        N = length(x);
        rho = 5E-3;
        nbItMax = 1000;
        
        for k = 1:2*N/N_decoupage -2
            X = x(debut:fin, 1);

            % a. Initialisation
            alpha = zeros(N_decoupage, 1);		% valeur initiale des parametres
            J(1) = (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1); % Valeur du critere pour les parametres initiaux
	
            % b. Iterations
            for ind = 2:nbItMax
			
                % Calcul du gradient
                gradJ = -2*D'*(X - D*alpha) + lambda*sign(alpha);
			
                % mise a jour des parametres
                alpha = alpha - rho*gradJ;
                J(ind) =  (X - D*alpha)'*(X - D*alpha) + lambda*norm(alpha, 1);
            end
    
            s_decoupe(:, k) = D*alpha;
            taux(k, 1) = nnz(alpha);
            
            debut = debut + N_decoupage/2;
            fin = fin + N_decoupage/2;
        end
        %{
        figure(2);
        plot((1:nbItMax), J(1:nbItMax));
        title('Evolution du critère');
        xlabel('nombre iteration');
        ylabel('critère');
        %}
            
        [N_ligne,N_colonne] = size(s_decoupe);

        for l = 1:N_colonne
            s_fenetre(:, l) = s_decoupe(:, l).*fenetre;
        end     
end

end
