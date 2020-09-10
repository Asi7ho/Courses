function spclab(varargin)
%spclab Speech signal presentation program.
%
%   Usage:
%   spclab(fs,filename) reads speech from the specified file (HP format default).
%   spclab(fs,filename1,filename2,filename3) : arbitrary many files is allowed.
%   spclab(fs,x) uses the vector x as indata.
%   spclab(fs,x,y,z) : : arbitrary many files is allowed.
%   spclab(fs,x,filename,y) : mix between filenames and variables.
%   spclab(x,filename,y) : all of the above options without specifying fs 
%                             (the default will be used)
%
%   fs=sampling frequency in Hz (default: 8000)
%
%   MOUSE CLICKS IN SPEECH PLOT
%   Left button:   Select a region of the signal.
%   Center button: Listen to the selected region on the DAT player.
%   Right button, short press: Zoom to the selected region, 
%         or zoom out if no region is selected.
%   Right button, long press: Function menu.
%   Slider: Move around in the file.
%
%   MOUSE CLICKS IN SPECTRUM PLOT
%   Right button: Harmonic cursor.
%
%
%   Some global constants can also be changed (the colors, autoscaling
%   on/off, play command, byte order, etc), by changing the constants 
%   in the beginnarging of this file. Look for the "user-defined constants"
%   section.
%
%   Author original version: Thomas Eriksson
%   Version: 2.0
%   Date: 98-12-19
%
%   Jan Plasberg 2004-01-01: Many changes to improve functionality
%       New spectrograms
%       Possibility to evaluate your own functions from within spclab
%       Deleting a plot made possible
%       Handling of the sampling rate
%   Bastiaan Kleijn 2004-11-15: smartreadspeechfile made smart
%   Thomas Eriksson 2005-03-29: Cosmetic changes, removal of _jp check :-)
%   Jan Plasberg 2005-06-13: New design, enhanced scaling features
%   Jan Plasberg 2005-10-21: updated UNIX playout (using 'play')


global spclab_play_machine 
global speech t maxy lengths
global hbox hline hspectrum hfigure hslider haxis htext
global startpt startxpos startypos menupos
global x1 x2 x1all x2all
global t2 y2
global mode initialized verbose
global hmenu
global speechaxiscolor speechfigurecolor speechplotcolor menucolor
global spectrumaxiscolor spectrumfigurecolor spectrumplotcolor spectrumplotcolor2 menucolor
global autoscale xautoscale soundautoscale figurename figure_number soundline playcommand byteorder fs fs_play enabledoubleclick
global hsline hstext X colorcounter filenames headerlength
global export 
global max_amplitude %max_array min_array av_data
global default_function_x default_function_x_y

NFFT8000=256; %reference fft length @ fs=8000Hz for spectrogram
NWINWB8000=64; %reference window length @ fs=8000Hz for wb-spectrogram
fsdefault=8000; %default sampling rate

if nargin<1,
    disp('type ''help spclab'' for help');
    return;
end;


%test whether first argument is the sampling freq
if isa(varargin{1},'double') & size(varargin{1})==size(1)
    fs=varargin{1};
else
    fs=fsdefault;
    disp(['Sampling rate set to ' num2str(fsdefault),' Hz (default).']);
    for i=length(varargin):-1:1
        varargin{i+1}=varargin{i};
    end;
    varargin{1}=fs;
end

if length((varargin{2}))~=1 | isstr(varargin{2})
    spclab(fs,1,'initialize');
    for i=2:length(varargin)
        if ~isstr(varargin{i})
            spclab(fs,1,'add',varargin{i},['arg ' char(48+i-1)]);
        else
            spclab(fs,1,'add',smartreadspeechfile(varargin{i}),varargin{i});
        end;
    end;
    return;
end;

switch varargin{3}
case 'initialize',
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
    % default constants
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % verbose mode on/off
    verbose = 0;
    
    % Colors of the speech plot (RGB format)
    speechaxiscolor=[1 1 1];
    speechfigurecolor=[0.92 0.9 0.9];
    speechplotcolor=[0 0 1];
    
    % Colors of the spectrum plot (RGB format)
    spectrumaxiscolor=[1 1 1];
    spectrumfigurecolor=[0.92 0.9 0.9];
    spectrumplotcolor=[0 0 1 ; 1 0 0 ; 0 1 0 ; 0 0 0; 1 1 0 ; 1 1 1];
    
    % Color of the popup menu
    menucolor=[0.82 0.8 0.8];
    
    % y-axis autoscaling
    % (0) all plots in the main window are scaled to the currently visible 
    %     maximum amplitude of all signals 
    % (1) each plot in the main window is scaled to the respective maximum
    %     amplitude  [-max(abs(x)) max(abs(x))]
    % (2) each plot is scaled for maximum visibility, i.e.,  [-min(x) max(x)]
    % (3) like mode (2), but with mean-removed plots
    %     (useful to look at signals with large mean and low variance)
    autoscale=1;
    
    % x-axis autoscaling on (1) or off (0). 
    % If on, everything fills the whole width of the screen
    xautoscale=1;
    
    % Autoscaling of the sound output on (1) or off (0).
    % if (1), sound is scaled according to input with max amplitude
    % if (0), sound is scaled for each file individually
    soundautoscale=0;
    
    max_amplitude=0;
    
    % Name of the main figure
    figurename='spclab'; 
    figure_number=1000;
    
    %play-out sampling frequency
    fs_play=44100;
    
    % UNIX command to play a file (this is a hack!)
    playcommand=[' fid=fopen([getenv(''HOME''),''/spclab_playout.sw''], ''w'');',...
        'fwrite(fid,playout*2^14,''int16'');',...
        'fclose(fid);',...
        '!play -r' num2str(fs_play) ' -c1 ',getenv('HOME'),'/spclab_playout.sw' ...
		    ' &'];
    % playcommand=['rsh ',spclab_play_machine,' sage ',getenv('HOME'),'/_temp.d'];
    
    % Byte order of the speech file. 'ieee-be' is HP byte order, 'ieee-le' is PC byte order (I think).
    byteorder='ieee-be';
    
    % headerlength of the speechfiles in characters
    headerlength=512;
    
    %default function calls for optional functions
    default_function_x = 'var(x);';
    default_function_x_y = 'x-y;';
    
    filenames=[];
    soundline=1;
    initialized=1;
    speech=[];
    lengths=[];
    haxis=[];
    hbox=[];
    hline=[];
    mode=0;
    
    %open a new figure window if needed
    if hfigure==[]
        hfigure=figure(figure_number);
    else
        try
            close(hfigure);
            hfigure=figure(figure_number);
        catch
            hfigure=figure(figure_number);
        end
    end
    clf;
    %set(hfigure,'numbertitle','off','name',figurename,'menubar','none');
    set(hfigure,'numbertitle','off','name',figurename);
    hspectrum=-1;
    
    x1all=1;x2all=1;
    x1=0;x2=0;
    
    % create a slider
    spclab(fs,1,'slider',0);
    
    set(hfigure,'windowbuttondownfcn',['spclab(',num2str(fs),',1,''mark'',1)']);
    set(hfigure,'color',speechfigurecolor);
    set(hfigure,'Position',  [180 220 700 500]);
    
    % create a quit button
    %     hslider=uicontrol('style','slider',...
    %             'units','normalized','position',[0 0 1 0.05],...
    %             'min',0,'max',1,'value',(x1all+x2all)/2/max(1,length(speech)),...
    %             'sliderstep',[0.2,1],...
    %             'backgroundcolor',speechfigurecolor,...
    %             'callback',['spclab(',num2str(fs),',1,''slider'',1)'],'visible','on');
    %     
    quitbutton = uicontrol('Style', 'pushbutton',...
        'String', 'Quit',...
        'units','normalized','position',[0 0.05 0.05 0.05],...
        'backgroundcolor',speechfigurecolor, ...
        'Callback', ['spclab(',num2str(fs),',1,''quit'',1)'],'visible','on');
    
    %    'Position', [1 30 30 30],
    
    % some information for the user:
    if verbose == 1
        if soundautoscale==1
            disp('soundautoscale=1: scaling sound output according to input with max amplitude.');
        else
            disp('soundautoscale=0: scaling sound output for each input individually.');
        end;
        if soundline==1
            disp('soundline=1: visualization of current position during sound output.');
        else
            disp('soundline=0: no visualization of current position during sound output.');
        end;
    end
        

    
case 'add',
    
    data=varargin{4}(:);
    lengths=[lengths; length(data)];
    %ensure right sound scaling for playout
    max_amplitude_data=max(abs(data));
    if (isempty(max_amplitude)) | (max_amplitude_data > max_amplitude)
        max_amplitude = max_amplitude_data;
    end;
    
    filenames=strvcat(filenames,varargin{5});
    s=size(speech);
    
    
    if s(1)<length(data)
        speech=[speech;zeros(length(data)-s(1),s(2))];
    else
        data=[data;zeros(s(1)-length(data),1)];
    end;
    speech=[speech data];
    
    x1all=1;x2all=length(speech);
    x1=0;x2=0;
    
    haxis(s(2)+1)=axes;

    delta=(1-0.07)/length(haxis);
    for i=1:length(haxis)
        set(haxis(length(haxis)+1-i),'position',[0.08,0.07+0.01+(i-1)*delta,0.91,delta-0.03]);
    end;
    
    spclab(fs,1,'zoom');
    
    
% take care of marking
case 'mark',
    ca=find(haxis==gca);
    if autoscale>0
        %in that case the marking box height for all axis 
        %was reduced to the current maxy, which is not intended, 
        %so do some rescaling...
        maxy=2*max(max(abs(speech)));
    end
    switch varargin{4}
    case 1,
        selectiontype=get(gcf,'selectiontype');
        switch selectiontype,
        case 'normal',
            enabledoubleclick=0;
            startpt=get(haxis(ca),'currentpoint');
            set(hbox,'xdata',[startpt(1,1) startpt(1,1) startpt(1,1) startpt(1,1)],...
                'ydata',[-maxy 2*maxy 2*maxy -maxy],...
                'visible','on');
            set(hline,'xdata',[startpt(1,1) startpt(1,1)],...
                'ydata',[-maxy 2*maxy],...
                'visible','on');
            set(gcf,'windowbuttonmotionfcn',['spclab(',num2str(fs),',1,''mark'',3)']);
            set(gcf,'windowbuttonupfcn',['spclab(',num2str(fs),',1,''mark'',2)']);
            set(gca,'units','pixels');
            axispos=get(gca,'position');
            set(gca,'units','normalized');
            htext=uicontrol('style','text','pos',[axispos(1)+2 axispos(2)+axispos(4)-22 80 20],'string',0,'back',speechaxiscolor/1.1);
        case 'extend',
            enabledoubleclick=0;
            spclab(fs,1,'sound',0);
        case {'alt','open'},
            if [enabledoubleclick==0 strcmp(selectiontype,'open')]
                return;
            end;
            enabledoubleclick=1;
            tic;
            set(gcf,'windowbuttonupfcn',['spclab(',num2str(fs),',1,''mark'',4)']);
            
            mode=1;
            pause(0.3);
            if  mode==1
                spclab(fs,1,'menu',1);
            else
                set(gcf,'windowbuttonupfcn','');
            end;
            mode=0;
        end;
    case 2,
        delete(htext);
        pt=get(haxis(ca),'currentpoint');
        set(hbox,'xdata',[startpt(1,1) startpt(1,1) pt(1,1) pt(1,1)],...
            'ydata',[-maxy maxy maxy -maxy],...
            'erasemode','xor');
        set(hline,'visible','off');
        if pt==startpt
            set(hbox,'visible','off');
            x1=0;
            x2=0;
        else
            x1=max(1,floor(fs*min(startpt(1,1),pt(1,1))));
            x2=min(length(speech),ceil(fs*max(startpt(1,1),pt(1,1))));
        end;
        set(gcf,'windowbuttonmotionfcn','');
        set(gcf,'windowbuttonupfcn','');
    case 3,
        pt=get(haxis(ca),'currentpoint');
        set(hbox,'xdata',[startpt(1,1) startpt(1,1) pt(1,1) pt(1,1)],...
            'ydata',[-maxy maxy maxy -maxy],...
            'erasemode','xor');
        set(hline,'visible','off');
        delta=1000*abs(startpt(1,1)-pt(1,1));
        if delta<1000
            set(htext,'string',[int2str(round(delta)) ' ms (' int2str(round(delta/1000*fs)) ')']);
        else
            set(htext,'string',round(delta)/1000);
        end;
    case 4,
        if mode==1
            if [(x1==0) (x2==0)]
                cent=(x1all+x2all)/2;
                range=x2all-x1all;
                x1all=round(max(1,cent-range));
                x2all=round(min(length(speech),cent+range));
            else
                x1all=x1;
                x2all=x2;
                x1=0;
                x2=0;
            end;
            mode=0;
            spclab(fs,1,'zoom');
            set(gcf,'windowbuttonupfcn','');
        end;
    end;
    
    
case 'zoom',
    x1=0;
    x2=0;
    if autoscale==1
        maxy=0;
    end;

    for i=1:length(haxis)
        if xautoscale==0
            axes(haxis(i));
            y=reduce(speech(x1all:x2all,i));
            if autoscale==1
                maxy=max(abs(y));
                if maxy<1e-10
                    maxy=1e-10;
                end
            else
                maxy=max(max(abs(speech(x1all:x2all,:))));
                if maxy<1e-10
                    maxy=1e-10;
                end
            end;
            t=(0:length(y)-1)*(x2all-x1all+1)/length(y)/fs+x1all/fs;

        else
            axes(haxis(i));

            if lengths(i)<max(lengths)
                uplen=x2all-x1all;
                ydown_temp=speech(1:lengths(i),i);
                usfac=floor(max(lengths)/lengths(i));
                yup_temp=ones(usfac,1)*ydown_temp';
                yup_temp=yup_temp(:);
                yup_temp=resample(yup_temp,max(lengths),length(yup_temp),0);
                y=reduce(yup_temp(x1all:x2all));
            else
                y=reduce(speech(x1all:x2all,i));
            end

            if autoscale==1
                maxy=max(abs(y));
                if maxy<1e-10
                    maxy=1e-10;
                end
            else
                maxy=max(max(abs(speech(x1all:x2all,:))));
                if maxy<1e-10
                    maxy=1e-10;
                end
            end;
            t=(0:length(y)-1)*(x2all-x1all+1)/length(y)/fs+x1all/fs;

        end
        if autoscale==3
            plot(t,y-mean(y),'color',speechplotcolor);
        else
            plot(t,y,'color',speechplotcolor);
        end

        ylabel(filenames(i,:));
        set(haxis(i),'color',speechaxiscolor);
        set(haxis(i),'FontName', 'Times','FontSize', [8]);
        hold on
        hbox(i)=fill([0 0 0 0],[0 0 0 0],'k','erasemode','xor','visible','off');
        hline(i)=line([0 0],[0 0],'color','k','erasemode','xor','visible','off');

        hold off
        if autoscale==0
            axis([-inf inf -maxy maxy]);
        elseif autoscale==1
            set(haxis(i),'xlim',[-inf inf],'ylim',[-maxy maxy]);
        elseif autoscale==2
            if max(y)-min(y)>1e-10
                set(haxis(i),'xlim',[-inf inf],'ylim',[min(y) max(y)]);
            else
                set(haxis(i),'xlim',[-inf inf],'ylim',[max(y)-1e-10 max(y)]);
            end
        elseif autoscale==3
            if max(y-mean(y))-min(y-mean(y))>1e-10
                set(haxis(i),'xlim',[-inf inf],'ylim',[min(y-mean(y)) max(y-mean(y))]);
            else
                set(haxis(i),'xlim',[-inf inf],'ylim',[max(y-mean(y))-1e-10 max(y-mean(y))]);
            end
            hold on
            plot(t,zeros(length(y),1),'r:')
            ylim=get(gca,'ylim');
            htext=text(t(1),(ylim(2)-ylim(1))*0.05,['mean:' num2str(mean(y))]);
            set(htext,'Color','r');
            hold off
        end;
    end; %for haxis

    %set(haxis,'xlim',[-inf inf],'ylim',[-maxy maxy]);
    spclab(fs,1,'slider',2);

% Display the right-click menu, and take care of the menu choice
case 'menu',
        if autoscale>0
            ca=find(haxis==gca);
            y=reduce(speech(x1all:x2all,ca));
        maxy=max(abs(y));
        if maxy<1e-10
            maxy=1e-10;
        end

    end
    
    pt=get(gca,'currentpoint');
    ylim=get(gca,'ylim');
    set(gca,'units','pixel');
    apos=get(gca,'pos');
    set(gca,'units','normalized');
    xpos=apos(1)+apos(3)*(pt(1,1)-t(1))/(t(length(t))-t(1));
    %ypos=apos(2)+apos(4)*(pt(1,2)+maxy)/2/maxy;
    ypos=apos(2)+apos(4)*(pt(1,2)-ylim(1))/(ylim(2)-ylim(1));
    
    
    switch varargin{4}
    case 1,
        hmenu=zeros(1,20);
        hmenu(1)=uicontrol('string','Zoom full out','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(2)=uicontrol('string','New spectrum','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(3)=uicontrol('string','Add spectrum','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(4)=uicontrol('string','Add LPC(10) spectrum','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(5)=uicontrol('string',['Switch y-autoscaling (' num2str(autoscale) ')'],'style','text','visible','off','backgroundcolor',menucolor);
        hmenu(6)=uicontrol('string','Toggle x-autoscaling','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(7)=uicontrol('string','Save marked','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(8)=uicontrol('string','Save marked as variable','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(9)=uicontrol('string','NB color spectrogram','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(10)=uicontrol('string','WB color spectrogram','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(11)=uicontrol('string','NB gray spectrogram','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(12)=uicontrol('string','WB gray spectrogram','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(13)=uicontrol('string','Autocorrelation','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(14)=uicontrol('string','Crosscorrelation','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(15)=uicontrol('string','Crossspectrum','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(16)=uicontrol('string','Cepstrum','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(17)=uicontrol('string','LPC residual','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(18)=uicontrol('string','Your function f(x)','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(19)=uicontrol('string','Your function f(x,y)','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(20)=uicontrol('string','Eval in base worksp.','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(21)=uicontrol('string','Delete row','style','text','visible','off','backgroundcolor',menucolor);
        hmenu(22)=uicontrol('string','Options','style','text','visible','off','backgroundcolor',menucolor);
%        hmenu(23)=uicontrol('string','Quit','style','text','visible','off','backgroundcolor',menucolor);
        startxpos=xpos;
        startypos=max(ypos,(length(hmenu)+1)*20);
        for i=1:length(hmenu)
            set(hmenu(i),'position',[startxpos startypos-20*i 120 20],...
                'visible','on');
        end;
        set(gcf,'windowbuttonmotionfcn',['spclab(',num2str(fs),',1,''menu'',2)']);
        set(gcf,'windowbuttonupfcn',['spclab(',num2str(fs),',1,''menu'',3)']);
        menupos=0;
    case 2,
        if [xpos>startxpos xpos<startxpos+120]
            oldmenupos=menupos;
            menupos=ceil((startypos-ypos)/20);
            if [menupos>0 menupos<=length(hmenu)]
                set(hmenu(menupos),'backgroundcolor','k','foregroundcolor','w');
            end;
            if [oldmenupos~=menupos oldmenupos>0 oldmenupos<=length(hmenu)]
                set(hmenu(oldmenupos),'backgroundcolor',menucolor,'foregroundcolor','k');
            end;
        else
            if [menupos>0 menupos<=length(hmenu)]
                set(hmenu(menupos),'backgroundcolor',menucolor,'foregroundcolor','k');
                menupos=0;
            end;
        end;
    case 3,
        set(gcf,'windowbuttonmotionfcn','');
        set(gcf,'windowbuttonupfcn','');
        if [xpos>startxpos xpos<startxpos+120]
            menupos=ceil((startypos-ypos)/20);
            menupos=max(1,menupos);
            menupos=min(length(hmenu),menupos);
            menuchoice=get(hmenu(menupos),'string');
            switch menuchoice
            case 'New spectrum',
                spclab(fs,1,'spectrum',1);
            case 'Add spectrum',
                spclab(fs,1,'spectrum',2);
            case 'Add LPC(10) spectrum',
                spclab(fs,1,'spectrum',3);
            case 'Zoom full out',
                x1all=1;
                x2all=length(speech);
                spclab(fs,1,'zoom');
            %case 'Toggle y-autoscaling',
            %    autoscale=1-autoscale;
            case ['Switch y-autoscaling (' num2str(autoscale) ')'],
                %switch through autoscaling modes 0,1,2,3
                autoscale=mod(autoscale+1,4);
                maxy=max(max(abs(speech)));
                spclab(fs,1,'zoom');
            case 'Toggle x-autoscaling',
                xautoscale=1-xautoscale;
                %maxy=max(max(abs(speech)));
                spclab(fs,1,'zoom');
            case 'Save marked',
                spclab(fs,1,'save',1);
            case 'Save marked as variable',
                spclab(fs,1,'save',2);
            case 'NB color spectrogram',
                spclab(fs,1,'spectrogram',1);
            case 'WB color spectrogram',
                spclab(fs,1,'spectrogram',2);
            case 'NB gray spectrogram',
                spclab(fs,1,'spectrogram',3);
            case 'WB gray spectrogram',
                spclab(fs,1,'spectrogram',4);
            case 'Autocorrelation',
                spclab(fs,1,'autocorrelation');
            case 'Crosscorrelation',
                spclab(fs,1,'crosscorrelation');
            case 'Crossspectrum',
                spclab(fs,1,'crossspectrum');
            case 'Cepstrum',
                spclab(fs,1,'cepstrum');
            case 'LPC residual',
                spclab(fs,1,'residual');
            case 'Your function f(x)',
                spclab(fs,1,'function_x');
            case 'Your function f(x,y)',
                spclab(fs,1,'function_x_y');
            case 'Eval in base worksp.',
                spclab(fs,1,'evalin_base');
            case 'Options',
                spclab(fs,1,'options');
            case 'Delete row'
                spclab(fs,1,'delete');
            case 'Quit',
                spclab(fs,1,'quit');
                %                 close('spclab');
                %                 if ishandle(hspectrum)
                %                     close(hspectrum);
                %                 end;
                return;
            end;
        end;
        for i=1:length(hmenu)
            delete(hmenu(i));
        end;
    end;
    
    
    % play sound
case 'sound',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    if [(x1==0) (x2==0)]
        data=speech(x1all:x2all,ca);
    else
        data=speech(x1:x2,ca);
    end;
    L=min(80,floor(length(data)/2));
    win=hanning(2*L);
    data(1:L)=data(1:L).*win(1:L);
    data(length(data)-L+1:length(data))=data(length(data)-L+1:length(data)).*win(L+1:2*L);
    
    if soundautoscale==1
        %do scaling according to max(max(abs))) over all files
        data=data/max_amplitude*0.9;
    else
        data=data/max(abs(data))*0.9;
    end;
    
    %mysound can do the job better
    mysound(data,fs);
    
     
    set(hfigure,'pointer','arrow');
    
    if soundline==1
        if [(x1==0) (x2==0)]
            t1=x1all/fs;
            t2=x2all/fs;
        else
            t1=x1/fs;
            t2=x2/fs;
        end;
        h=line([t1,t2],[-maxy,maxy],'color','k','erasemode','xor');
        tic;
        time=toc;
        while time<t2-t1
            set(h,'xdata',[t1+time,t1+time]);
            time=toc;
        end;
        set(h,'visible','off');
    end;
 
 
    % plot spectrum
case 'spectrum',
    switch varargin{4}
    case {0,1,2,3},
        ca=find(haxis==gca);
        set(hfigure,'pointer','watch');
        if [(x1==0) (x2==0)]
            cutout=speech(x1all:x2all,ca).*hamming(x2all-x1all+1);
        else
            cutout=speech(x1:x2,ca).*hamming(x2-x1+1);
        end;
        switch varargin{4}
        case {1,2}
            X=20*log10(abs(fft(cutout,2^ceil(log2(length(cutout)))))+eps);
        case 3,
            [a,g]=lpc(cutout,10);
            X=20*log10(g+eps)-20*log10(abs(fft(a,256))+eps);
        end;
        X=reduce(X(1:floor(length(X)/2)+1));
        f=(0:length(X)-1)*fs/2/(length(X)-1);
        if varargin{4}==1
            hspectrum=-1;
        end;
        if ishandle(hspectrum)
            figure(hspectrum);
            hold on;
            plot(f,X,'color',spectrumplotcolor(colorcounter+1,:));
            hold off;
        else
            hspectrum=figure;
            colorcounter=0;
            plot(f,X,'color',spectrumplotcolor(colorcounter+1,:));
            set(gcf,'position',[10 10 450 250],'color',spectrumfigurecolor);
            set(gca,'units','normalized','position',[0.06 0.06 0.94 0.93],'color',spectrumaxiscolor);
        end;
        colorcounter=mod(colorcounter+1,length(spectrumplotcolor));
        set(gcf,'windowbuttondownfcn',['spclab(',num2str(fs),',1,''spectrum'',4)']);
        set(gca,'FontName', 'Times','FontSize', [8]);
        set(hfigure,'pointer','arrow');
        zoom on;
    case 4,
        selectiontype=get(gcf,'selectiontype');
        switch selectiontype,
        case 'alt',
            pt=get(gca,'currentpoint');
            hsline=zeros(1,10);
            temp=get(gca,'ylim');
            harm1=pt(1,1)/length(hsline);
            for i=1:length(hsline)
                hsline(i)=line([i*harm1 i*harm1],[temp(1) temp(2)],'erasemode','xor','color',[1 1 1]-spectrumplotcolor(colorcounter));
            end;
            set(gcf,'windowbuttonmotionfcn',['spclab(',num2str(fs),',1,''spectrum'',5)']);
            set(gcf,'windowbuttonupfcn',['spclab(',num2str(fs),',1,''spectrum'',6)']);
            
            set(gca,'units','pixels');
            axispos=get(gca,'position');
            set(gca,'units','normalized');
            temp2=pt(1,1)/length(hsline);
            hstext=uicontrol('style','text','pos',[axispos(1)+axispos(3)-121 axispos(2)+axispos(4)-21 120 20],'string',[int2str(temp2) ' Hz (' int2str(fs/temp2) ' samples)'],'back',spectrumaxiscolor/1.1);
        end;
    case 5,
        pt=get(gca,'currentpoint');
        harm1=pt(1,1)/length(hsline);
        for i=1:length(hsline)
            set(hsline(i),'xdata',[i*harm1 i*harm1]);
        end;
        temp2=pt(1,1)/length(hsline);
        set(hstext,'string',[int2str(temp2) ' Hz (' num2str(round(4*fs/temp2)/4) ' samples)']);
    case 6,
        for i=1:length(hsline)
            delete(hsline(i));
        end;
        set(gcf,'windowbuttonmotionfcn','');
        set(gcf,'windowbuttonupfcn','');
        delete(hstext);
    end;	
    
case 'spectrogram',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    
    figure;
    switch varargin{4}
    case {1,2},
        if [(x1==0) (x2==0)]
        cutout=speech(x1all:x2all,ca).*hamming(x2all-x1all+1);
    else
        cutout=speech(x1:x2,ca).*hamming(x2-x1+1);
    end;
        if varargin{4}==1
            NFFT=2^ceil(log2(NFFT8000*fs/8000));
            HL=NFFT;
            %NFFT=256; HL=256;
        else
            NFFT=2^ceil(log2(NFFT8000*fs/8000));
            HL=2^ceil(log2(NWINWB8000*fs/8000));
            %NFFT=256; HL=64;
        end;  
        WINDOW=hanning(HL);
        UPDATE=2^ceil(log2(2*fs/8000));
        NOVERLAP=HL-UPDATE;
        specgram(cutout,NFFT,fs,WINDOW,NOVERLAP);
        set(gca,'FontName', 'Times','FontSize', [8]);
    case {3,4},
        if [(x1==0) (x2==0)]
        cutout=speech(x1all:x2all,ca);
    else
        cutout=speech(x1:x2,ca);
    end;
        if varargin{4}==3
            NFFT=2^ceil(log2(NFFT8000*fs/8000));
            NWIND=NFFT;
            %NFFT=256; NWIND=256;
        else
            NFFT=NFFT8000*round(fs/8000);
            NWIND=2^ceil(log2(NWINWB8000*fs/8000));
            %NFFT=256; NWIND=64;
        end;  
        UPDATE=2^ceil(log2(2*fs/8000));
        NOVERLAP=NWIND-UPDATE;
        subplot(211);
        
        [sgr,frs,tms]=specgram(filter([1 -0.9],1,cutout),NFFT,fs,NWIND,NOVERLAP);
        subplot('position',[0.05 0.2 0.9 0.8])
        imagesc(tms,frs,20*log10(abs(sgr)+eps));
        set(gca,'FontName', 'Times','FontSize', [8]);
        axis xy;
        colormap(gray(256));colormap(1-colormap);brighten(0.6);
        
        [row,col]=size(sgr);
        clear sgr;
        tt = 1/fs:1/fs:(col-1)*(NWIND-NOVERLAP)/fs;
        
        
        subplot('position',[0.05 0.02 0.9 0.11])
        plot(tt,cutout(1:length(tt)));
        set(gca,'YTick',[]); 
        set(gca,'XTickLabel',[]);
        axis([tt(1) tt(length(tt)) min(cutout)*1.04-eps max(cutout)*1.04+eps]);
        clear tt frs tms; 
        
        
        
        
        
    end
    
    set(gcf,'numbertitle','off','name',filenames(ca,:));
    zoom on;
    set(hfigure,'pointer','arrow');
    
    %    ca=find(haxis==gca);
    %    set(hfigure,'pointer','watch');
    %    if [(x1==0) (x2==0)]
    %        cutout=speech(x1all:x2all,ca).*hamming(x2all-x1all+1);
    %    else
    %        cutout=speech(x1:x2,ca).*hamming(x2-x1+1);
    %    end;
    %    figure;
    %    if varargin{4}==1
    %        NFFT=256;
    %        NOVERLAP=NFFT/2;
    %    else
    %        NFFT=64;
    %        NOVERLAP=60;
    %    end;  
    %    specgram(cutout,NFFT,fs,[],NOVERLAP);
    %    set(gcf,'numbertitle','off','name',filenames(ca,:));
    %    zoom on;
    %    set(hfigure,'pointer','arrow');
    
case 'autocorrelation',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    if [(x1==0) (x2==0)]
        cutout=speech(x1all:x2all,ca);
    else
        cutout=speech(x1:x2,ca);
    end;
    figure;
    R=xcorr(cutout);
    R=reduce(R);
    tim=((0:length(R)-1)-(length(R)-1)/2)./fs;
    plot(tim,R);
    axis([tim(1) tim(end) -inf inf])
    zoom on;
    set(gcf,'position',[10 10 450 250],'color',spectrumfigurecolor);
    set(gcf,'numbertitle','off','name',filenames(ca,:));
    set(gca,'color',spectrumaxiscolor);
    
    set(hfigure,'pointer','arrow');
    
case 'cepstrum',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    if [(x1==0) (x2==0)]
        cutout=speech(x1all:x2all,ca).*hamming(x2all-x1all+1);
    else
        cutout=speech(x1:x2,ca).*hamming(x2-x1+1);
    end;
    R=real(ifft(log(abs(fft(cutout)).^2)+eps));
    R=reduce(R(1:floor(length(R)/2)+1));
    tim=(0:length(R)-1)./fs;
    figure;
    plot(tim,R);
    axis([tim(1) tim(end) -inf inf])
    zoom on;
    set(gcf,'position',[10 10 450 250],'color',spectrumfigurecolor);
    set(gcf,'numbertitle','off','name',filenames(ca,:));
    set(gca,'color',spectrumaxiscolor);
    set(hfigure,'pointer','arrow');
    
case 'crosscorrelation',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    if [(x1==0) (x2==0)]
        cutout1=speech(x1all:x2all,ca);
        cutout2=speech(x1all:x2all,1);
    else
        cutout1=speech(x1:x2,ca);
        cutout2=speech(x1:x2,1);
    end;
    figure;
    R=xcorr(cutout1,cutout2);
    R=R/sqrt((cutout1'*cutout1)*(cutout2'*cutout2));
    R=reduce(R);
    tim=((0:length(R)-1)-(length(R)-1)/2)./fs;
    plot(tim,R);
    axis([tim(1) tim(end) -inf inf])
    zoom on;
    set(gcf,'position',[10 10 450 250],'color',spectrumfigurecolor);
    set(gcf,'numbertitle','off','name', [filenames(ca,:), ' & ', filenames(1,:)] );
    set(gca,'color',spectrumaxiscolor);
    
    set(hfigure,'pointer','arrow');
    
case 'crossspectrum',
    ca=find(haxis==gca);
    set(hfigure,'pointer','watch');
    if [(x1==0) (x2==0)]
        cutout1=speech(x1all:x2all,ca);
        cutout2=speech(x1all:x2all,1);
    else
        cutout1=speech(x1:x2,ca);
        cutout2=speech(x1:x2,1);
    end;
    figure;
    R=xcorr(cutout1,cutout2);
    R=R/sqrt((cutout1'*cutout1)*(cutout2'*cutout2));
    X=20*log10(abs(fft(R,2^ceil(log2(length(R)))))+eps);
    X=reduce(X(1:floor(length(X)/2)+1));
    f=(0:length(X)-1)*fs/2/(length(X)-1);
    plot(f,X);
    axis([f(1) f(end) -inf inf])
    zoom on;
    set(gcf,'position',[10 10 450 250],'color',spectrumfigurecolor);
    set(gcf,'numbertitle','off','name',[filenames(ca,:), ' & ', filenames(1,:)] );
    set(gca,'color',spectrumaxiscolor);
    
    set(hfigure,'pointer','arrow');
    
    % take care of slider
case 'slider',
    switch varargin{4}
        
        % slider initialization
    case 0,
        hslider=uicontrol('style','slider',...
            'units','normalized','position',[0 0 1 0.05],...
            'min',0,'max',1,'value',(x1all+x2all)/2/max(1,length(speech)),...
            'sliderstep',[0.2,1],...
            'backgroundcolor',speechfigurecolor,...
            'callback',['spclab(',num2str(fs),',1,''slider'',1)'],'visible','on');
        % slider has changed, update plot
    case 1,
        range=x2all-x1all+1;
        center=get(hslider,'value')*length(speech);
        x1all=round(min(length(speech)-range+1,max(1,center-range/2)));
        x2all=round(x1all+range-1);
        spclab(fs,1,'zoom');
        
        % plot has changed, update slider
    case 2,
        range=(x2all-x1all+1)/length(speech);
        set(hslider,'sliderstep',[range/5 range],'value',(x1all+x2all)/2/length(speech));
    end;
    
case 'slider2',
    switch varargin{4}
        
        % slider initialization
    case 0,
        axes('units','normalized','position',[0.05 0 0.9 0.05]);
        uicontrol('units','normalized','position',[0,0,0.05,0.05]);
        uicontrol('units','normalized','position',[0.95,0,0.05,0.05]);
        
        % initiate limits and figure
    case 1,
        
        % slider has changed, update plot
    case 2,
        range=x2all-x1all+1;
        center=get(hslider,'value')*length(speech);
        x1all=round(min(length(speech)-range+1,max(1,center-range/2)));
        x2all=round(x1all+range-1);
        spclab(fs,1,'zoom');
        
        % plot has changed, update slider
    case 3,
        range=(x2all-x1all+1)/length(speech);
        set(hslider,'sliderstep',[range/5 range],'value',(x1all+x2all)/2/length(speech));
    end;
    
case 'residual',
    ca=find(haxis==gca);
    sp=speech(:,ca);
    res=zeros(size(sp));
    N=floor(length(sp)/160);
    Z=[];
    for i=1:N
        frame=sp((i-1)*160+1:i*160);
        a=lpc(frame,10);
        [frameres,Z]=filter(real(a),1,frame,Z);
        res((i-1)*160+1:i*160)=frameres;
    end;
    speech(:,ca)=res;
    spclab(fs,1,'zoom');
    
    % save the file
case 'save',
    ca=find(haxis==gca);
    if [x1==0 x2==0]
        data=speech(x1all:x2all,ca);
    else 
        data=speech(x1:x2,ca);
    end;
    switch varargin{4}
    case 1,
        [Name,Path]=uiputfile('export.mat','Save As');
        if [Name~=0 Path~=0]
            %fid=fopen([Path Name],'w',byteorder);
            %fwrite(fid,data,'short');
            %fclose(fid);
            export=data;
            save([Path Name],'export');
        end;
    case 2,
     %      disp('Data in variable named "export"');
     %      disp('(you must type "global export" before exporting)');
     %      export=data;
     
     %exportvar=input('\n Saving data in a new worspace variable. \n Variable name (<enter> for default ´export´): ','s');
     exportvar=inputdlg('Please enter variable name: ','Save to workspace variable',1,{'export'},'off');     
     if length(exportvar)==0
         return
     end
     if (length(exportvar{1})==0)|(isstr(exportvar{1})==0)
         spclab(fs,1,'save',2);
     else
         assignin('base',exportvar{1},data);
     end;
     
    end;
    
    % try to open file
    
    
case 'function_x'
    ca=find(haxis==gca);
    if [x1==0 x2==0]
        start=x1all;
        stop=x2all;
    else 
        start=x1;
        stop=x2;
    end;
    dlgtitle='Function call f(x)';
    dlgprompts={'Please enter the function name.',['argument x (1..' num2str(size(speech,2)) ')']};
    dlgdefaults={default_function_x,num2str(ca)};
    answer=inputdlg(dlgprompts,dlgtitle,1,dlgdefaults,'off');     
    if length(answer)==0
        return;
    end;
    func=answer{1};
    x=speech(start:stop,str2num(answer{2}));
    
    success=1;
    
    eval(['result=' func],'success=0;');
        
    try
        if success==1
            if length(result)>0
                assignin('base','f_x_result',result);
                
                if size(result)==size(1) & isnumeric(result)
                    Message={'The result is f(x) =' num2str(result) ,...
                            '(workspace variable ''f_x_result'').'};
                    Title='Function call f(x)';
                    msgbox(Message,Title);
                elseif size(result(:),1)<=size(x,1)+100 & isnumeric(result) & isreal(result)
                    result=result(:);
                    %newline=[zeros(start-1,1); result; zeros(size(x,1)-stop,1)];
                    newline=[zeros(start,1); result; zeros(lengths(ca)-start-length(result),1)];
                    spclab(fs,1,'add',newline,['arg ' char(48+size(speech,2))]);
                    return
                    
                else
                    Message={'The result is complex, struct or cell array',...
                            '(workspace variable ''f_x_result'').'};
                    Title='Function call f(x)';
                    msgbox(Message,Title);
                end
            end
        else
            Message={lasterr};
            Title='Error Message';
            msgbox(Message,Title);
        end
    catch
        Message={['Probably you used an uninitialized internal spclab variable name.      Check warnings in command window.' lasterr]};
        Title='Error Message';
        msgbox(Message,Title);
    end
    
case 'function_x_y'
    ca=find(haxis==gca);
    if [x1==0 x2==0]
        start=x1all;
        stop=x2all;
    else 
        start=x1;
        stop=x2;
    end;
    dlgtitle='Function call f(x,y)';
    dlgprompts={'Please enter the function name (or any MATLAB code).',['argument x (1..' num2str(size(speech,2)) ')'] ,['argument y (1..' num2str(size(speech,2)) ')']};
    dlgdefaults={default_function_x_y,'1',num2str(ca)};
    answer=inputdlg(dlgprompts,dlgtitle,1,dlgdefaults,'off');     
    if length(answer)==0
        return;
    end;
    func=answer{1};
    x=speech(start:stop,str2num(answer{2}));
    y=speech(start:stop,str2num(answer{3}));


    success=1;
    eval(['result=' func],'success=0;');
    
    try
        if  success==1 
            if length(result)>0
                assignin('base','f_x_y_result',result);
                
                if size(result)==size(1) & isnumeric(result)
                    Message={'The result is f(x,y) =' num2str(result) ,...
                            '(workspace variable ''f_x_y_result'').'};
                    Title='Function call f(x,y)';
                    msgbox(Message,Title);
                elseif size(result(:),1)<=size(x,1)+100 & isnumeric(result) & isreal(result)
                    result=result(:);
                    %newline=[zeros(start-1,1); result, zeros(size(x,1)-stop,1)];
                    newline=[zeros(start,1); result; zeros(lengths(ca)-start-length(result),1)];
                    spclab(fs,1,'add',newline,['arg ' char(48+size(speech,2))]);
                    return
                    
                else
                    Message={'The result is complex, struct or cell array',...
                            '(workspace variable ''f_x_y_result'').'};
                    Title='Function call f(x,y)';
                    msgbox(Message,Title);
                end
            end
        else
            Message={lasterr};
            Title='Error Message';
            msgbox(Message,Title);
        end
    catch
        Message={'Probably you used an uninitialized internal spclab variable name.      Check warnings in command window.'};
        Title='Error Message';
        msgbox(Message,Title);
    end
    
case 'evalin_base'
    dlgtitle='Evaluation in MATLAB base workspace.';
    dlgprompts={'Please enter the string to evaluate. Tip: use a variable name to load the variable into spclab.'};
    dlgdefaults={''};
    answer=inputdlg(dlgprompts,dlgtitle,1,dlgdefaults,'off');     
    if length(answer)==0
        return;
    end;
    func=answer{1};
    
    try
        result=evalin('base',func);
        success=1;
    catch
        Message={lasterr};
        Title='Error Message';
        msgbox(Message,Title);
        success=0;
    end
            
    
    if  success==1 
        if length(result)>0
            assignin('base','evalin_base_result',result);
            
            if size(result)==size(1) & isnumeric(result)
                Message={'The result is ' num2str(result) ,...
                        '(workspace variable ''evalin_base_result'').'};
                Title='Evaluation in MATLAB base workspace';
                msgbox(Message,Title);
            elseif isnumeric(result)
                newline=result(:);
                spclab(fs,1,'add',newline,['arg ' char(48+size(speech,2))]);
                return
                
            else
                Message={'The result is a struct or a cell array',...
                        '(workspace variable ''evalin_base_result'').'};
                Title='Evaluation in MATLAB base workspace';
                msgbox(Message,Title);
            end
        end
    end
     
case 'delete',
    ca=find(haxis==gca);
    s=size(speech);
    
    if s(2)==1
        disp('Press ''Quit'' if you want to quit!');
        return;
    else
        keep=ones(s(2),1);
        keep(ca)=0;
        delete(haxis(ca));
        %shorten resp. arrays
        haxis=haxis(find(keep==1));
        speech=speech(:,find(keep==1));    
        hbox=hbox(find(keep==1));
        hline=hline(find(keep==1));
    
        delta=(1-0.07)/length(haxis);
        for i=1:length(haxis)
            set(haxis(end+1-i),'position',[0.08,0.07+0.01+(i-1)*delta,0.91,delta-0.03]);
        end;
    
        % create a figure
        maxy=max(1,max(max(abs(speech))));
        max_amplitude=maxy;
        spclab(fs,1,'zoom');
    end

        
case 'options',   
    edit spclab;
    return; 
        
case 'quit',   
    close(figure_number);
    if ishandle(hspectrum)
        close(hspectrum);
    end;
    return; 
    
end;


function out=reduce(x, N)

if nargin==1
    N=5000;
end;

if length(x)>4*N
    L=floor(length(x)/N);
    x1=[x;x(length(x))*ones(ceil(length(x)/L)*L-length(x),1)];
    x2=reshape(x1,L,ceil(length(x)/L));
    out=reshape([max(x2);min(x2)],2*length(x2),1);
else
    out=x;
end;




function out=readspeechfile(name,byteorder)

fid=fopen(name,'r',byteorder);
if fid>0
    out=fread(fid,inf,'short');
    fclose(fid);
else
    disp(['Cannot open file "' name '"']);
end;





function out=smartreadspeechfile(name)
global verbose;
global headerlength;

fid=fopen(name);
fclose(fid);
if fid>0
    sz=5*512;
    E1=0;
    fid=fopen(name,'r','ieee-be');
    x=fread(fid,sz,'short');
    fclose(fid);
    fid=fopen(name,'r','ieee-le');
    y=fread(fid,sz,'short');
    fclose(fid);
    E1=x(257:length(x))'*x(257:length(x));
    E2=y(257:length(y))'*y(257:length(y));
    if E1<E2
        byteorder='ieee-be';
        if max(abs(x(1:256)))>5000
            if verbose==1
                disp('HP format file, with header');
            end
            header=1;
        else
            if verbose==1
                disp('HP format file, without header');
            end
            header=0;
        end;
    else
        byteorder='ieee-le';
        if max(abs(y(1:256)))>5000
            if verbose==1
                disp('PC format file, with header');
            end
            header=1;
        else
            if verbose==1
                disp('PC format file, without header');
            end
            header=0;
        end;
    end;
    fid=fopen(name,'r',byteorder);
    if header==1
        if verbose==1
            disp(['Using header length ' num2str(headerlength) ' (to change search for the setting headerlength=...)']);
        end
        fread(fid,headerlength,'short');
    end;
    out=fread(fid,inf,'short');   
    fclose(fid);
else
    
    disp(['Cannot open file "' name '"']);
    out=[];
end;

function mysound(data, fs)
% playout routine for windows (and unix)

%default input sampling frequency
if nargin<2
    fs=8000;
end;

%playout sampling frequency
global fs_play; %=44100;
global playcommand;

%scaling for integer data
if max(abs(data)) > 1
    data=data/2^15;
end;

if fs ~= fs_play
    playout=resample(data,fs_play,fs);
else
    playout=data;
end;

if ispc
    sound(playout,fs_play);
else
   eval(playcommand);
end