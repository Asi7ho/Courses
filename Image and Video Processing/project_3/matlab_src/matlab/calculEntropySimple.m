function L = calculEntropySimple(I, q)
    
    vals = I(:);
    bins = min(vals):q:max(vals);
    
    if (length(bins) > 1)
        p = hist(vals, bins);
        p = p/sum(p);
        L = -sum(p.*log2(p+eps));
    else
        L = 0;
    end

end