function S=rand(mc,T)
%S=rand(mc,T) returns a random state sequence from given MarkovChain object.
%
%Input:
%mc=    a single MarkovChain object
%T= scalar defining maximum length of desired state sequence.
%   An infinite-duration MarkovChain always generates sequence of length=T
%   A finite-duration MarkovChain may return shorter sequence,
%   if END state was reached before T samples.
%
%Result:
%S= integer row vector with random state sequence,
%   NOT INCLUDING the END state,
%   even if encountered within T samples
%If mc has INFINITE duration,
%   length(S) == T
%If mc has FINITE duration,
%   length(S) <= T
%
%---------------------------------------------
%Code Authors:
%---------------------------------------------

S=zeros(1,T);%space for resulting row vector
nS=mc.nStates;
reachEnd = 0;
tEnd = 0;

% t = 1
p = DiscreteD(mc.InitialProb);
State = rand(p, 1);

% t > 2 
for t = 2:T+1
    
    if State == nS + 1
        reachEnd = 1;
        tEnd = t-1;
        break;
    end
    
    S(t-1) = State;
   
    p = DiscreteD(mc.TransitionProb(S(t-1), :));
    
    State = rand(p, 1);
    
end

if reachEnd == 1
   S = S(1:tEnd-1); 
end

end


