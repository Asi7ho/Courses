function d = dqpsk(b)

    QPSK = qpsk(b);
    N = length(QPSK);
    
    d(1) = QPSK(1);
    
    for i = 2:N
       d(i) = QPSK(i)*QPSK(i-1);
    end

end