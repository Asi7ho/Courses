function MX = MakeMXMatrix(x_input, d, k, nf)
    nlen = size(x_input, 2);
    MX = zeros((nlen-k+1)*nf, k*nf*d);
    
    vectX = x_input(:);
    
    for i = 0:nlen-k
        start = i*d + 1;
        Lined = vectX(start:start+d*k-1)';
        MX(i*nf+1:(i+1)*nf, :) = kron(eye(nf), Lined);
    end
end