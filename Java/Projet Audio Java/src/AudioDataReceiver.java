import phelmaaudio.AudioData;
import phelmaaudio.WavFileException;

import java.io.IOException;

public class AudioDataReceiver extends ModuleAbstract {

    private phelmaaudio.AudioData conteneur;

    public AudioDataReceiver(String nom) {
        super(nom, 1, 1);
        conteneur = new AudioData();
    }

    public void saveAudioDataToWavFile(String audioFileName) {
        try {
            conteneur = new AudioData(audioFileName);
        } catch (IOException e){
            System.out.println("Erreur d'entrée sortie");
        } catch (WavFileException w) {
            System.out.println("Erreur de fichier");
        }
    }

    public void displayAudioDataWaveform() {
        conteneur.display();
    }

    public void playAudioData() {
        conteneur.play();
    }


    @Override
    public void exec() {
        double e = this.getInputPortValue(0);
        conteneur.addSample(e);
        setAndSendOutputPortValue(0, e);
        //System.out.println(this.getInputPortValue(0));
    }

    @Override
    public void reset() {
        this.setInputPortValue(0,0);
        conteneur.reset();
    }
}
