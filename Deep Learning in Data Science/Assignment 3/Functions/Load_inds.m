function [X_train_ids, y_train_ids, X_val_ids, y_val_ids] = Load_inds(all_names, ys)

X_val_ids = dlmread("Validation_Inds.txt");
y_val_ids = ys(X_val_ids);

X_train_ids = 1:length(all_names);
X_train_ids(X_val_ids) = [];
y_train_ids = ys(X_train_ids);

end