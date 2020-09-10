function [Xb, Yb, MXb] = BalancedData(X, Y, MX)

%     count = histcounts(ys);
%     [min_class, ~] = min(count);
%     
%     l = min_class*size(Y, 1);
%     
%     Xb = zeros(size(X, 1), l);
%     Yb = zeros(size(Y, 1), l);
%     MXb = cell(l, 1);
%     
%     for i = 1:size(Y, 1)
%         [Yb(:, (i-1)*min_class + 1:i*min_class), idx] =  datasample(Y(:, ys' == i), min_class, 2, 'Replace',false);
%         Xb(:, (i-1)*min_class + 1:i*min_class) = X(:, idx);
%         MXb((i-1)*min_class + 1:i*min_class) = MX(idx);
%         
%     end
%     
%     perm = randperm(l, l);
%     Xb = Xb(:, perm); 
%     Yb = Yb(:, perm);
%     MXb = MXb(perm);
    
    [ys, ~] = find(Y);
    
    count = histcounts(ys);
    [min_class, ~] = min(count);
    
    idx = ceil(rand(1, min_class) .* count(1));
    
    cumNclass = count(1);
    
    for i=2:size(Y, 1)
        idx = [idx, ceil(rand(1, min_class) .* count(i) + cumNclass)];
        cumNclass = cumNclass + count(i);
    end
    
    idx = idx(randperm(size(idx, 2)));
    
    Xb = X(:, idx);
    Yb = Y(:, idx);   
    MXb = MX(idx);


end