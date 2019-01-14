function [BuffResiduals, DxDy, Lmv, Lres] = MotionEncoder(Luminance, Dict, range, A, M, MotionX, MotionY)
CountMoves = zeros(Dict.Count, 1);
for l = 2:size(Luminance, 1)
    PreviousFrame = Luminance{l-1, 1};
    CurrentFrame = Luminance{l, 1};
    for i = 1:size(CurrentFrame, 1)/16
        for j =  1:size(CurrentFrame, 2)/16
            [dx, dy] = MotionCopensated(PreviousFrame, CurrentFrame, [16*(i-1)+1:16*i; 16*(j-1)+1:16*j], MotionX, MotionY);
            DxDy{l-1, 1}{i, j} = [dx, dy];
            CountMoves(Dict(num2str([dx, dy])), 1) = CountMoves(Dict(num2str([dx, dy])), 1) + 1;
            Icompensated{l, 1}(16*(i-1)+1:16*i, 16*(j-1)+1:16*j) = PreviousFrame(16*(i-1)+1+dy:16*i+dy, 16*(j-1)+1+dx:16*j+dx);
        end
    end
    Iresiduals{l, 1} = CurrentFrame - Icompensated{l, 1};
end

for k = range
    for l = 2:size(Luminance, 1)
        I = Iresiduals{l, 1};
        IntraRes = IntraFrame(I, A, M);
        IntraqRes = MbyMblocked(uniformQuantizer(cell2mat(IntraRes), 2^k), 16);
        BuffResiduals{l-1, k-(range(1))+1} = IntraqRes;
    end
end


p = CountMoves/sum(CountMoves);
Lmv = -log2(p+eps);

Lres = LengthBuff(BuffResiduals, range);
end