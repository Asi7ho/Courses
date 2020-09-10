function CM = confusionMatrix(resLabel, TestSet, numClass)

    CM = zeros(numClass);
    
    for r = 1:length(resLabel)
       Truelabel = str2double(extractBefore(TestSet(r).name, "-"));
       CM(resLabel(r), Truelabel) = CM(resLabel(r), Truelabel) + 1;
    end
    
end