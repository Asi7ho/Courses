function newPitch = removeNoiseAndSilent(frIsequence)

    P = log(medfilt1(frIsequence(1,:)));
    R = frIsequence(2,:);
    I = log(frIsequence(3,:));

    lwb = 0;

    % Normalize
    In = (I - min(I))/(max(I) - min(I));
   
    % Threshold
    thI = mean(In);
    thR = 0.8;
    thP = mean(P) + std(P);

    
    newPitch = P;

    newPitch(In < thI) = lwb;
    newPitch(R < thR) = lwb;
    newPitch(P > thP) = lwb;
 
end