function DecodeInter = DecodeInterFrame(Buff, A, M, range)

for k = range
    for l = 1:size(Buff, 1)
        I = MbyMblocked(Buff{l, k-(range(1))+1}, 16);
        for i = 1:size(Buff{l, k-(range(1))+length(range)+1}, 1)
             for j = 1:size(Buff{l, k-(range(1))+length(range)+1}, 2)
                 if Buff{l, k-(range(1))+length(range)+1}(i, j) == 0
                     Idec{i, j} = I{i, j};
                 elseif Buff{l, k-(range(1))+length(range)+1}(i, j) == 1
                     Idec{i, j} = cell2mat(DecodeIntraFrame(I{i, j}, A, M));
                 else
                     Idec{i, j} = I{i, j};
                 end
             end
        end
        DecodeInter{l, k-(range(1))+1} = cell2mat(Idec);
    end
end

end