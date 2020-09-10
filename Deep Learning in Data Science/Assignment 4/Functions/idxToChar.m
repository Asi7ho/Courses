function txt = idxToChar(X, book_chars)

    for i = 1:size(X, 2)
       txt(i) = book_chars(find(X(:, i)));
    end

end