function [dx, dy] = MotionCopensated(Iprev, Icurr, blockCoordinates, MotionX, MotionY)

min = inf;

if blockCoordinates(1, 1) - abs(MotionY(1)) < 0
    MotionY = MotionY(abs(MotionY(1)) - blockCoordinates(1, 1) + 2):MotionY(end);
end

if blockCoordinates(2, 1) - abs(MotionX(1)) < 0
    MotionX = MotionX(abs(MotionX(1)) - blockCoordinates(2, 1) + 2):MotionX(end);
end

if blockCoordinates(1, end) + MotionY(end) > size(Icurr, 1)
    MotionY = MotionY(1):MotionY(blockCoordinates(1, end) +  MotionY(end) - size(Icurr, 1) + 1);
end

if blockCoordinates(2, end) + MotionX(end) > size(Icurr, 2)
    MotionX = MotionX(1):MotionX(blockCoordinates(2, end) + MotionX(end) - size(Icurr, 2) + 1);
end
    

for i = MotionX
    for j = MotionY
        d = immse(Iprev(blockCoordinates(1, :)+j, blockCoordinates(2, :)+i), Icurr(blockCoordinates(1, :), blockCoordinates(2, :)));
        if d < min
            min = d;
            dx = i;
            dy = j;
        end
    end
end

end