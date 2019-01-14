function [Buff, CoI] = ConditionalFrame(Luminance, A, M, range, L)

CoI = zeros(length(range), 2);

for k = range
    
    %First frame
    I = Luminance{1, 1};
    
    CurrentFrame = IntraFrame(I, A, M);
    Buff{1, k-(range(1))+1} = uniformQuantizer(cell2mat(CurrentFrame), 2^k);
    Buff{1, k-(range(1))+length(range)+1} = ones(size(CurrentFrame, 1), size(CurrentFrame, 2));
    
    CoI(k-(range(1))+1, 2) = CoI(k-(range(1))+1, 2) + 11*9;
    
    %NextFrames
    for l = 2:size(Luminance, 1)
        PreviousFrame = MbyMblocked(Luminance{l-1, 1}, 16);
        CurrentFrame = MbyMblocked(Luminance{l, 1}, 16);
        
        CurrentIntra = IntraFrame(cell2mat(CurrentFrame), A, M);
        CurrentIntraq = MbyMblocked(uniformQuantizer(cell2mat(CurrentIntra), 2^k), 16);
        
        for i = 1:size(CurrentFrame, 1)
            for j = 1:size(CurrentFrame, 2)
                
                J_copy = LagragianCost(CurrentFrame{i, j}, PreviousFrame{i ,j}, 2^k, 1, 1);
                J_intra = LagragianCost(CurrentIntra{i, j}, CurrentIntraq{i ,j}, 2^k, sum(sum(L{l, k-(range(1))+1}{i, j})) + 1, 1);
                
                if J_copy < J_intra
                    IBuff{i, j} = PreviousFrame{i, j};
                    CopyOrIntra(i, j) = 0;
                    CoI(k-(range(1))+1, 1) = CoI(k-(range(1))+1, 1) + 1;
                else
                    IBuff{i, j} = CurrentIntraq{i, j};
                    CopyOrIntra(i, j) = 1;
                    CoI(k-(range(1))+1, 2) = CoI(k-(range(1))+1, 2) + 1;
                end
            end
        end
        
        Buff{l, k-(range(1))+1} = cell2mat(IBuff);
        Buff{l, k-(range(1))+length(range)+1} = CopyOrIntra;
        
    end
    
end
end