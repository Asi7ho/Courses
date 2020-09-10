function hmm = training(D, winlength)

    pD = GaussMixD;
    
    obsData = [];

    for r = 1:size(D, 1)

        [y, Fs]=audioread([D(1).folder, '/', D(r).name]);
        
        frIsequence = GetMusicFeatures(y, Fs, winlength);
        features = PostProcess(frIsequence);
        lData(r) = size(features, 2);
        
        obsData = [obsData, features];
        
    end
    
    hmm = MakeLeftRightHMM(ceil(max(obsData)), pD, obsData, lData);

end