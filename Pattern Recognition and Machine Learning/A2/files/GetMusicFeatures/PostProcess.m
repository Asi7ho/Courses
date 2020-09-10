function feature = PostProcess(features)
    newPitch = removeNoiseAndSilent(features);
    newPitch = addNoise(newPitch, 0.2);
    newPitchHz = exp(newPitch);
    feature = transformSemitone(newPitchHz);
end