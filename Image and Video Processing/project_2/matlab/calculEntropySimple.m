function E = calculEntropySimple(I, q)
    
    vals = I(:);
    bins = min(vals):q:max(vals);
    
    if (length(bins) > 1)
        p = hist(vals, bins);
        p = p/sum(p);
        E = -sum(p.*log2(p+eps));
    else
        E = 0;
    end

end