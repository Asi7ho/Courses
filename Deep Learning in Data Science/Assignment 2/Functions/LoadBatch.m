function [X, Y, y] = LoadBatch(filename)
    X = double(filename.data')./255;
    y = filename.labels;
    Y = (y == 0:max(y))';
end