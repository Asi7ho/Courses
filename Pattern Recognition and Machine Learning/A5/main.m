close all;
clear all;

addpath('Dataset')
addpath('PattRecClasses-2')
addpath('GetMusicFeatures')
addpath('Training')

%% Dataset 

%%%%%%% Train Set %%%%%%%
Dataset = {'1 - France Anthem', 
     '2 - Sweden Anthem', 
     '3 - Portugal Anthem',
     '4 - Germany Anthem',
     '5 - England Anthem',
     '6 - USA Anthem',
     '7 - Russia Anthem',
     '8 - Greece Anthem',
     '9 - Belgium Anthem',
     '10 - China Anthem'};
 
for i = 1:size(Dataset, 1)
   TrainSet{i, 1} = dir(['Dataset/', Dataset{i}, '/*.wav']);  
end

disp('Train Set Loaded')

%%%%%%% Test Set %%%%%%%
TestSet = dir(['Dataset/Tests', '/*.wav']);
disp('Test Set Loaded')

%% Training process

winlength = 0.03;

disp('Begin training process')
f = waitbar(0,'1','Name','Training...');

for i = 1:size(Dataset, 1)
    
    waitbar(i/size(Dataset, 1), f, sprintf('Class %d', i))
    
    hmm(i) = training(TrainSet{i, 1}, winlength); 
end

delete(f)

save('hmm')

disp('End of training process')

%% Test process

disp('Test on the Test Set')

obsData = [];

for r = 1:size(TestSet, 1)
    [y, Fs]=audioread([TestSet(1).folder, '/', TestSet(r).name]);
    frIsequence=GetMusicFeatures(y, Fs, winlength);
    obsData = PostProcess(frIsequence);
    
    lP = logprob(hmm, obsData);
    [logp,result] = max(lP);
    
    resLabel(r) = result;
    
    fprintf('The file %s is from the melody : %s\n',  TestSet(r).name, Dataset{result})
end

CM = confusionMatrix(resLabel, TestSet, 10);
Acc = sum(diag(CM))/size(TestSet, 1);