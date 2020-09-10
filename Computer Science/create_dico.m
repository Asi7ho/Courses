function D = create_dico(N, type_de_decomposition)

% N : la dimension du dictionnaire
% type_de_decomposition : permet de choisir le dictionnaire
switch type_de_decomposition

    case 'unitaire_cos'
        
        for i = 1:N
            for j = 1:N
                D(i, j) = sqrt(2/N)*(1/sqrt(1+eq(i,1)))*cos(pi*(2*j-1)*(i-1)/(2*N));
            end
        end
        
    case 'unitaire_rand'
        
        A = randn(N);
        A = A'*A;
        [D, Diag] = eig(A);
        
    case 'cos_non_unitaire'
        for i = 1:N
            for j = 1:N
                D(i, j) = sqrt(2/N)*(1/sqrt(1+eq(i,1)))*cos(pi*(2*j-1)*(i-1)/(2*N));
            end
        end
        D = D.*window(@triang, N);
        
    case 'cos_sin_non_unitaire'
        for i = 1:N
            for j = 1:N
                D(i, j) = sqrt(2/N)*(1/sqrt(1+eq(i,1)))*cos(pi*(2*j-1)*(i-1)/(2*N)) + sqrt(2/N)*(1/sqrt(1+eq(i,1)))*sin(pi*(2*j-1)*(i-1)/(2*N));
            end
        end
        
    case 'random_dico'
        D = randn(N);
        
end

end