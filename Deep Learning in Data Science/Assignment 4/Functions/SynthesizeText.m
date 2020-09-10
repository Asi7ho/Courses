function Y = SynthesizeText(RNN, h0, x0, n, book_chars, dataset)

    h = h0;
    x = x0;
    
    K = size(RNN.U, 2);
    
    Y(:, 1) = x;

    for i = 2:n
       a = RNN.U*x + RNN.W*h + RNN.b;
       h = tanh(a);
       o = RNN.V*h + RNN.c;
       p = exp(o)./sum(exp(o));
      
       cp = cumsum(p);
       a = rand;
       ixs = find(cp-a > 0);
       ii = ixs(1); 

       Y(:, i) = zeros(K, 1);
       Y(ii, i) = 1;
       x = Y(:, i);
       
       if dataset == "Trump"
           if idxToChar(x, book_chars) == '±'
               break;
           end
       end 
    end
end