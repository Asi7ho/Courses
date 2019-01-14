function DecodeCopy = DecodeConditionalFrame(Buff, A, M, range)

for k = range
    for l = 1:size(Buff, 1)
        I = MbyMblocked(Buff{l, k-(range(1))+1}, 16);
        for i = 1:size(Buff{l, k-(range(1))+length(range)+1}, 1)
             for j = 1:size(Buff{l, k-(range(1))+length(range)+1}, 2)
                 if Buff{l, k-(range(1))+length(range)+1}(i, j) == 0
                     Idec{i, j} = I{i, j};
                 else
                     Idec{i, j} = cell2mat(DecodeIntraFrame(I{i, j}, A, M));
                 end
             end
        end
        DecodeCopy{l, k-(range(1))+1} = cell2mat(Idec);
    end
end

end