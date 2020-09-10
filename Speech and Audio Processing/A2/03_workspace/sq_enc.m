function idx = sq_enc(in, n_bits, xmax, m)

L = 2^n_bits - 1;
delta = 2*xmax/L;

idx = floor((sign(in).*min(xmax+m, sign(in).*in) + m)/delta);

end