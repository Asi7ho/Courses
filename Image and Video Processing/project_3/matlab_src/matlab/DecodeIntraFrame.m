function DecodeIntra = DecodeIntraFrame(I, A, M)

Iblock = MbyMblocked(I, 16);

for i = 1:size(Iblock, 1)
    for j = 1:size(Iblock, 2)
        Iblock2{i, j} = MbyMblocked(Iblock{i, j}, M);
    end
end

for i = 1:size(Iblock2, 1)
    for j = 1:size(Iblock2, 2)
        Idct{i, j} = DCTTransformInverse(Iblock2{i, j}, A);
    end
end

for i = 1:size(Idct, 1)
    for j = 1:size(Idct, 2)
        DecodeIntra{i, j} = cell2mat(Idct{i, j});
    end
end

end