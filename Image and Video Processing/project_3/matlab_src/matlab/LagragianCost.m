function J = LagragianCost(I1, I2, Q, R, Mode)

if Mode == 1
    lambda = 0.0001*Q^2;
else
    lambda = 0.0004*Q^2;
end
D = immse(I2, I1);
J = D + lambda*R;

end