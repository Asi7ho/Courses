clear all
close all

%% Rentrer un signal

Fe = 48e3; %Fréquence d'échantillonnage du signal
t = 0:1/Fe:1; %Traitement pour une seconde de signal

f0 = 100;
f1 = 1000;
f2 = 5000;

w0 = 2*pi*f0;
w1 = 2*pi*f1;
w2 = 2*pi*f2;


%% Appliquer un filtre passe bas

wc_passe_bas = w0; %pulsation de coupure
spb = cos(w0*t) + cos(w2*t) + 1;
filtre_passe_bas(spb, t, Fe, wc_passe_bas);

%% Appliquer un filtre passe haut

wc_passe_haut = w2; %Fréquence de coupure
sph = cos(w0*t) + cos(w2*t) + 1;
filtre_passe_haut(sph, t, Fe, wc_passe_haut);


%% Appliquer un filtre passe bande

w0_passe_bande = w1; %Fréquence à garder
Q_pb = 10; %Facture de qualité

sp_b = cos(w0*t) + cos(w1*t) +cos(w2*t);
filtre_passe_bande(sp_b, t, Fe, w0_passe_bande, Q_pb);


%% Appliquer un filtre coupe bande

w0_coupe_bande = w1; %Fréquence à rejeter
Q_cb = 10; %Facture de qualité

sc_b = cos(w0*t) + cos(w1*t) +cos(w2*t);
filtre_coupe_bande(sc_b, t, Fe, w0_coupe_bande, Q_cb);

%% ADSR Numerique Lineaire

A = 0.2;
D = 0.3;
S = 0.3;
R = 0.2;

s2 = sin(w0*t);
    
ADSR(s2, t, Fe, A, D, S, R);

%% ADSR Numerique exponnentielle

A = 0.3;
D = 0.3;
S = 0.2;
R = 0.2;

s2 = sin(w0*t);
    
ADSR_exp(s2, t, Fe, A, D, S, R);