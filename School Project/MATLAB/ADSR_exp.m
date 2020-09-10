function ADSR_exp(x, t, Fe, A, D, S, R)

ti = fliplr(t); 

Attack = A*Fe;
Decay = Attack + D*Fe;
Sustain = Decay + S*Fe;
Relay = Sustain + R*Fe;

s_A = (1 - exp(-5*t/A))/(1-exp(-5));
s_D = exp(log(0.5)*t/D);
s_S = 0.5;
s_R = 0.5*exp(-5)*exp(5*ti/R);



for i = 1:Attack
    y(i) = x(i)*s_A(i);
end

for i = Attack + 1:Decay
    y(i) = x(i)*s_D(i - Attack);
end

for i = Decay + 1:Sustain
    y(i) = x(i)*s_S;
end

for i = Sustain + 1:Relay
   y(i) = x(i)*s_R(i); 
end

for i = Relay + 1:length(t)
   y(i) = 0; 
end

[up,lo] = envelope(y, 2000);

figure(2)
subplot(2,2,2)
plot(t, x)
title('s(t) = cos(2*pi*100*t)')
subplot(2,2,4)
plot(t, y) 
hold on
plot(t,up,t,lo, 'linewidth', 2)
title('enveloppe ADSR exp')
sound(y)


end