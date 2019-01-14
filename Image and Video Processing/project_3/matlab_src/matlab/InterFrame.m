function [Buff, CoIoM] = InterFrame(Luminance, A, M, range, BuffResiduals, DxDy, Dict, Lres, Lmv, Lintra)

CoIoM = zeros(length(range), 3);

for k = range
    
    %First frame
    I = Luminance{1, 1};
    
    CurrentFrame = IntraFrame(I, A, M);
    Buff{1, k-(range(1))+1} = uniformQuantizer(cell2mat(CurrentFrame), 2^k);
    Buff{1, k-(range(1))+length(range)+1} = ones(size(CurrentFrame, 1), size(CurrentFrame, 2));
    
    CoIoM(k-(range(1))+1, 2) = CoIoM(k-(range(1))+1, 2) + 11*9;
    
    %NextFrames
    for l = 2:size(Luminance, 1)
        PreviousFrameM = Luminance{l-1, 1};
        PreviousFrame = MbyMblocked(Luminance{l-1, 1}, 16);
        CurrentFrame = MbyMblocked(Luminance{l, 1}, 16);
        
        CurrentIntra = IntraFrame(cell2mat(CurrentFrame), A, M);
        CurrentIntraq = MbyMblocked(uniformQuantizer(cell2mat(CurrentIntra), 2^k), 16);
        
        for i = 1:size(CurrentFrame, 1)
            for j = 1:size(CurrentFrame, 2)
                
                dx = DxDy{l-1, 1}{i, j}(1);
                dy = DxDy{l-1, 1}{i, j}(2);
                CurrentMotionq(16*(i-1)+1:16*i, 16*(j-1)+1:16*j) = PreviousFrameM(16*(i-1)+1+dy:16*i+dy, 16*(j-1)+1+dx:16*j+dx) + cell2mat(DecodeIntraFrame(BuffResiduals{l-1, k-(range(1))+1}{i, j}, A, M));
                
                J_copy = LagragianCost(CurrentFrame{i, j}, PreviousFrame{i ,j}, 2^k, 2, 2);
                J_intra = LagragianCost(CurrentIntra{i, j}, CurrentIntraq{i ,j}, 2^k, sum(sum(Lintra{l, k-(range(1))+1}{i, j})) + 2, 2);
                J_motion = LagragianCost(CurrentFrame{i, j}, CurrentMotionq(16*(i-1)+1:16*i, 16*(j-1)+1:16*j), 2^k, sum(sum(Lres{l-1, k-(range(1))+1}{i, j})) + Lmv(Dict(num2str([dx, dy]))) + 2, 2);
                
                
                if (J_copy < J_intra && J_intra < J_motion) || (J_copy < J_motion && J_motion < J_intra)
                    IBuff{i, j} = PreviousFrame{i, j};
                    CopyOrIntraOrMotion(i, j) = 0;
                    CoIoM(k-(range(1))+1, 1) = CoIoM(k-(range(1))+1, 1) + 1;
                elseif (J_intra < J_copy && J_copy < J_motion) || (J_intra < J_motion && J_motion < J_copy)
                    IBuff{i, j} = CurrentIntraq{i, j};
                    CopyOrIntraOrMotion(i, j) = 1;
                    CoIoM(k-(range(1))+1, 2) = CoIoM(k-(range(1))+1, 2) + 1;
                else
                    IBuff{i, j} = CurrentMotionq(16*(i-1)+1:16*i, 16*(j-1)+1:16*j);
                    CopyOrIntraOrMotion(i, j) = 2 + Lmv(Dict(num2str([dx, dy])));
                    CoIoM(k-(range(1))+1, 3) = CoIoM(k-(range(1))+1, 3) + 1;
                end
            end
        end
        
        Buff{l, k-(range(1))+1} = cell2mat(IBuff);
        Buff{l, k-(range(1))+length(range)+1} = CopyOrIntraOrMotion;
        
    end
    
end
end