function bhat = detect_dqpsk(r)

ph(1) = r(1);

for i = 2:length(r)
    ph(i) = r(i)/conj(ph(i-1));
end

bhat = detect(ph);

    
end