function dPic = decoder(key, cPic)

%DECODER Decodes a color image encrypted with pixel permutation
%
%   DPIC = DECODER(KEY, CPIC) decodes the color image CPIC encrypted with
%   the function ENCODER(). The function takes the permutation coded image
%   CPIC and the decoding KEY, which contains a training sequence and the
%   permutation indices. If no output is specified, the decoded image is 
%   in shown as figure.
%
%   Input:
%   cPic    - encoded picture (maximium size 1024x1024)
%   key     - decoding key with a training sequence (values from {-1,1})
%   
%   Output:
%   dPic    - decoded picture (same size as cPic)
%
%   Version 2008, Signal Theory, Royal Institute of Technology (KTH)



%
% Parameter verification
%
precision = 10; % Sets maxmimum image size.

[x, y, z] = size(cPic);

if((x == 1) || (y == 1)) 
    error('Usage: decoder(key, cPic)');
end

if((x > 2^precision) || (y > 2^precision))
    error('The input image is too large, the maximum size is 1024x1024.');
end

[kx, ky] = size(key);
if((kx ~= 1) && (ky ~= 1)) 
    error('The key needs to be vector, not a matrix.');
end

training_length = 32;
if(kx*ky < (x+y+z)*precision+training_length) 
    error(sprintf('The key is too short, the length should be %d.',(x+y+z)*precision+training_length));
end
if(kx*ky > (x+y+z)*precision+training_length)
    error(sprintf('The key is too long, the length should be %d.',(x+y+z)*precision+training_length));
end

key = key(1+training_length : (x+y+z)*precision+training_length);
if( norm(sign(key)-key)>1e-3)
    error('The key should only contain values from the set {-1,1}');
end
key = sign(key); % Make sure it has +1 and -1 values



%
% Obtain the permutations
%
key_binary = (reshape(key,[precision (x+y+z)])' + 1) / 2;
key_binary = num2str(key_binary);
key_decimal = bin2dec(key_binary) + 1;

[temp px] = sort(key_decimal(1:x));
[temp py] = sort(key_decimal((x+1):(x+y)));
[temp pz] = sort(key_decimal((x+y+1):(x+y+z)));


%
% Recover the picture and show it
%

dPic = cPic(px,py,pz);

if (nargout==0)
    figure;
    image(dPic);
    axis square;
end

