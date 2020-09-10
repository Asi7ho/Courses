function MF = MakeMFMatrix(F, nlen)
    [dd, k, nf] = size(F);
    MF = zeros((nlen-k+1)*nf, nlen*dd);

    vectF = zeros(nf, dd*k);
    for j = 1:nf
        v = F(:, :, j);
        vectF(j, :) = v(:)';
    end
    
    for i = 0:nlen-k
       start = i*dd + 1;
       Lined = zeros(nf, nlen*dd);
       Lined(:, start:start+dd*k-1) = vectF;
       MF(i*nf+1:(i+1)*nf, :) = Lined;
    end
    
end