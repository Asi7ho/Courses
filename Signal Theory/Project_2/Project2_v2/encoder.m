function [key, cPic] = encoder(pic)

%ENCODER Encodes a color image by permuting the pixels randomly
%
%   [KEY,CPIC] = ENCODER(PIC) encodes the color image PIC generated with 
%   the function PIC=IMREAD(). The output image CPIC is a randomly permuted
%   version of PIC, with the exact permutation saved in binary form in KEY,
%   represented as values in the set {-1,1}. The first 32 elements of KEY
%   are training data, for the purpose of channel estimation and error
%   correction. The reverse operation is done by PIC=DECODER(KEY,CPIC)
%
%   Input:
%   pic     - the color image (maximium size 1024x1024)
%   
%   Output:
%   cPic    - encoded picture (same size as pic)
%   key     - decoding key with a training sequence
%
%   Version 2008, Signal Theory, Royal Institute of Technology (KTH)


%
% Generate a random permutation encoding
%
precision = 10; % Sets maxmimum image size.

[x, y, z] = size(pic);

if((x > 2^precision) || (y > 2^precision))
    error('The input image is too large, the maximum size is 1024x1024.');
end

px = randperm(x);
py = randperm(y);
pz = randperm(z);

cPic = pic(px,py,pz);


%
% Compute the decoding key in a nice way.
%
key_decimal = [px'; py'; pz']-1;
key_binary = dec2bin(key_decimal,precision)';
key_binary_unimodal  = double(key_binary(:))-48;
key_binary_unimodal = 2*key_binary_unimodal-1;


%
% Generate training data with as a constant vector using fixed random seed
%
training_length = 32;
rand('twister',6327);
training = sign(2*rand(training_length,1)-1);
rand('twister',sum(100*clock))


key = [training; key_binary_unimodal];

      