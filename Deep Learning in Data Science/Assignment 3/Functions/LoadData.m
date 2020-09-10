function [X, ys, all_names] = LoadData(data_fname)

[all_names, ys] = ExtractNames(data_fname);

C = unique(cell2mat(all_names')); 
d = numel(C);

n_len = size(char(all_names), 2);

for i = 1:length(all_names)
    Onehot = [(all_names{i} == C') zeros(d, n_len - length(all_names{i}))];
    X(:, i) = Onehot(:);
end

end