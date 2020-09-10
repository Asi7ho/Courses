package phelmaaudio;


import java.util.*;
import javax.sound.sampled.*;
import java.io.*;

/** lecteur de fichiers audio. F Miller */
public class AudioDataPlayer extends Thread {
	String titre;
	public AudioDataPlayer(String filename){
		titre = filename;
	}
	
	public AudioDataPlayer(AudioData data) {
		try {
			// generation d'un nom de fichier temporaire
			String property = "java.io.tmpdir";

		    String tempDir = System.getProperty(property);
		    System.out.println("OS current temporary directory is " + tempDir);
		    
			File f = File.createTempFile(tempDir + "tmpAudioFile",".wav");
			String tmpFileName = f.getAbsolutePath();
			f.delete();
			System.out.println("** wavfile " + tmpFileName);

			data.saveAudioDataToWavFileNormalized(tmpFileName);		
			titre = tmpFileName;
		} catch(Exception e) {
			throw new RuntimeException("cannot play audio " + e + e.getMessage());
		}
	}

	public void  run() {
		AudioInputStream stream;
		AudioFormat format;
		Clip clip;
		File object = new File(titre);
		try {
			stream = AudioSystem.getAudioInputStream(object);
			format = stream.getFormat();
			DataLine.Info info = new DataLine.Info(
					Clip.class, 
					stream.getFormat(), 
					((int) stream.getFrameLength() *
							format.getFrameSize()));

			clip = (Clip) AudioSystem.getLine(info);
			//clip.addLineListener(this);
			clip.open(stream);
			clip.start();
			try { sleep(99); } catch (Exception e) { }
			while (clip.isActive()) {
				try { sleep(99); } catch (Exception e) {break;}
			}
			clip.stop();
			clip.close();

		} catch (Exception ex) { 
			ex.printStackTrace(); 
		}
	}
}