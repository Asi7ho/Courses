function L = LengthBuff(Buff, range)
for k = range
    for i = 1:size(Buff{1,1}, 1)
        for j = 1:size(Buff{1,1}, 2)
            for q = 1:16
                for w = 1:16
                    for l = 1:size(Buff, 1)
                        vals(l) = Buff{l, k-(range(1))+1}{i, j}(q, w);
                    end
                    bins = min(vals):2^k:max(vals);
                    if (length(bins) > 1)
                        p = hist(vals, bins);
                        p = p/sum(p);
                        [dict, av] = huffmandict(bins,p);
                    else
                        dict = {0, 0};
                    end
                    for l = 1:size(Buff, 1)
                        L{l, k-(range(1))+1}{i, j}(q, w) = calculLength(vals(l), dict);
                    end
                end
            end
        end
    end
end
end