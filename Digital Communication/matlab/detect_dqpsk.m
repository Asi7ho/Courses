function bhat = detect_dqpsk(r)

angle(1) = r(1);

for i = 2:length(r)
    angle(i) = r(i)/angle(i-1);
end

bhat = detect(angle);

    
end