function Iblock = MbyMblocked(I, M)
% I : image
% M : size block
    [L, C] = size(I);
    Iblock = mat2cell(I, M*ones(L/M, 1), M*ones(C/M, 1));
end