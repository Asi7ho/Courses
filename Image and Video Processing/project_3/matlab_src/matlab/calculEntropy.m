function R = calculEntropy(I, q, M)

for i = 0:M-1
    for j = 0:M-1
        H(i+1,j+1) = calculEntropySimple(I(1+i:8:end, 1+j:8:end), q);
    end
end

R = mean2(H);

end