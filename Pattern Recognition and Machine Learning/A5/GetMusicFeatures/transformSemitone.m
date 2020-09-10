function semitone = transformSemitone(pitch)
    
    th = mean(pitch) - std(pitch);
    
    harmonics = pitch(pitch > th);
    
    f0 = min(harmonics);
    
    semitone = 12*log2(pitch/f0);
end