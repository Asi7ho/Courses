function outq = sq_dec(idx, n_bits, xmax, m)

L = 2^n_bits - 1;
delta = 2*xmax/L;

outq = delta*idx + delta/2 - m;

end