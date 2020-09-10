function M = ConfusionMatrix(P, Y)

    K = size(P, 1);
    M = zeros(K, K);

    for y=1:size(Y, 2)
        ind = find(Y(:, y));
        [~, i] = max(P(:, y));
        M(ind, i) = M(ind, i) + 1;
    end

end