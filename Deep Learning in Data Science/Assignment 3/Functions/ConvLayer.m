function S = ConvLayer(X, F, nlen)
     MF = MakeMFMatrix(F, nlen);
     S = max(0, MF * X);
end