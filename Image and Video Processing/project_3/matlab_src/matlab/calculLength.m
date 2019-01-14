function L = calculLength(val, dict)

for k = 1:size(dict, 1)
    if dict{k, 1} == val
       L = length(dict{k, 2});
       break;
    else
        L = 0;
    end
end

end