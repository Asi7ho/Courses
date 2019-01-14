function Dict = DictMov(MotionX, MotionY)

i = 1;
for x = MotionX
    for y = MotionY
        PossibleMoves{i} = num2str([x, y]);
        MovesValue(i) = i;
        i = i+1;
    end
end

Dict = containers.Map(PossibleMoves, MovesValue);

end