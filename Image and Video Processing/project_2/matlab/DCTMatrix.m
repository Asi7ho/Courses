function A = DCTMatrix(M)
% M : size of blocs

    for i = 0:M-1
        for k = 0:M-1
            if ( i == 0)
                A(i+1,k+1) = sqrt(1/M)*cos((2*k+1)*i*pi/(2*M));
            else
                A(i+1,k+1) = sqrt(2/M)*cos((2*k+1)*i*pi/(2*M));
            end
        end
    end
end