function mysound(data, fs)
%MYSOUND  Playout routine for Windows (and Unix)
%         MYSOUND(Y) sends the signal in vector Y to the sound system.
%         Sample values outside the range -32768,32767 are clipped.
%         The samples in Y may be floats (or doubles) but are converted
%         to 16 bit samples before they are played back.
%         The sound is played at the default sample rate of 8000 Hz.
%
%         MYSOUND(Y,FS) plays the sound at a sample rate of FS Hz.
%         
%         © Jan Plasberg and Jonas Samuelsson, 2005

% playout routine for windows (and unix)

%default input sampling frequency
if nargin<2
    fs=8000;
end;

%playout sampling frequency
fs_play = 44100;
playcommand=[' fid=fopen([getenv(''HOME''),''/mysound_playout.sw''], ''w'');',...
        'fwrite(fid,playout,''int16'');',...
        'fclose(fid);',...
        '!play -r' num2str(fs_play) ' -c1 ',getenv('HOME'),'/mysound_playout.sw' ...
		    ' &'];

idx = find(data > 32767);
data(idx) = 32767;
idx = find(data < -32768);
data(idx) = -32768;

if fs ~= fs_play
  playout=resample(data,fs_play,fs);
else
  playout=data;
end;

if ispc
  playout=playout/2^15;
  sound(playout,fs_play);
else
   eval(playcommand);
end
