function newPitch = addNoise(pitch, stdnoise)

    newPitch = pitch;
    
    th = mean(pitch) - std(pitch);
    
    harmonics = pitch(pitch > th);
    
    f0 = min(harmonics);

    lwb = min(pitch);
    
    virtualNoise = f0 + stdnoise*randn(1, length(pitch == lwb));
    newPitch(pitch == lwb) = virtualNoise(pitch == lwb);

end