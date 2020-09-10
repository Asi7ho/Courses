package phelmaaudio;

import java.util.*;
import javax.swing.JFrame;
import java.io.*;				// pour les IOEXception


/** 
 * Conteneneur d'échantillons audio  
 * (c'est à dire de signal audio) 
 * échantillonné à 44100 Hz.
 * @author N castagné
 */
public class AudioData {
	public static final int SAMPLERATE = 44100;
	private ArrayList<Double> data; /** la collection d'échantillons */

	// paramètres calculés, qui stockent le min et le max des data
	// pour optimiser les méthodes getMax(), getMin() et getMaxValAbs()
	private double minSampleValue;
	private double maxSampleValue;

	/** Construit un signal vide */
	public AudioData() {
		data = new ArrayList<Double>();
		maxSampleValue = 0;
		minSampleValue = 0;
	}

	/** Construit un signal lu depuis un  fichier audio au format .wav nommé fileName*/
	public AudioData(String fileName) throws IOException, WavFileException {
		this();
		loadFromWavFile(fileName);
	}

	/** Constructeur de copie */
	public AudioData(AudioData other) {
		data = new ArrayList<Double>(other.data); // Double immutable => pas besoin de copier les Double
		minSampleValue = other.minSampleValue;
		maxSampleValue = other.maxSampleValue;
	}

	/** nombre d'échantillons du signal audio */
	public int getNbSamples() {
		return data.size();
	}


	/** Ajoute un nouvel échantillon à la fin du signal audio */
	public void addSample(double sample) {
		data.add(sample);
		minSampleValue = Math.min(sample, minSampleValue);
		maxSampleValue = Math.max(sample, maxSampleValue);
	}

	/** Retourne le id-ième échantillon du signal audio */
	public double getSample(int id) {
		return data.get(id);
	}

	/** Remet à zéro le signal audio contenu dans l'objet */
	public void reset() {
		data.clear();
		maxSampleValue = 0;
		minSampleValue = 0;
	}

	/** Normalise les echantillons à la valeur maxAbsValue
	 * @param maxAbsValue : valeur max après normalisation. Doit être entre 0 et 1.0
	 */
	public void normalize(double maxAbsValue) {
		if(maxAbsValue < 0 || maxAbsValue > 1) {
			throw new IllegalArgumentException("Facteur de normalisation incorrect");
		}
		double maxAbs = getMaxValAbs();
		if(maxAbs == 0){
			return; // nothing to normalize
		}
		for(int i = 0 ; i < data.size(); i ++) {
			data.set(i, data.get(i) * maxAbsValue / maxAbs);
		}
		minSampleValue = minSampleValue * maxAbsValue / maxAbs;
		maxSampleValue = maxSampleValue * maxAbsValue / maxAbs;
	}

	/** Normalise les echantillons entre à la valeur 1.0 */
	public void normalize() {
		normalize(0.9);
	}

	/** Retourne true si le signal est compris entre -1.0 et 1.0 */
	public boolean isNormalized() {
		if( getMaxValAbs() > 1.0) {
			return false;
		}
		return true;
	}



	/** Retourne la valeur de l'échantillon de valeur absolue la plus grande. 
	 * Utile pour normalisation du signal entre -1.0 et 1.0
	 * */
	public double getMaxValAbs(){
		return  Math.max(Math.abs(minSampleValue), Math.abs(maxSampleValue));
	}

	/** Retourne la valeur du plus petit échantillon du signal */
	public double getMin(){
		return minSampleValue;
	}

	/** Retourne la valeur du plus grand échantillon du signal */
	public double getMax(){
		return maxSampleValue;
	}


	/** Joue le signal : envoie le signal 
	 *  sur la carte son de la machine, à la fréquence de 44100 echantillons / seconde.
	 *  EXception RuntimeException si problème.
	 */
	public void play() {
		AudioDataPlayer player = new AudioDataPlayer(this);
		player.start(); // lance la lecture audio
	}

	/** Créee une fenêtre et affiche la forme d'onde du signal 
	 * */
	public void display() {
		WaveformFrame frame = new WaveformFrame(this);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true); // affiche la fenêtre
	}


	/** Créee une fenêtre et affiche dans cette fenêtre, l'une au dessus de l'autre
	 * toutes les forme d'onde des signaux de la liste listOfAudioData
	 * @param listOfAudioData : les signaux à afficher
	 * */
	public static void displayMultipleAudioData(List<AudioData> listOfAudioData) {
		WaveformFrame frame = new WaveformFrame(listOfAudioData);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true); // affiche la fenêtre
	}


	/** Charge le signal du fichier audio au format .wav nommé fileName. 
	 * Si le fichier est multicanal, toutes les voies seront mixées en une seule.
	 * Si le fichier n'est pas à 44100 hz, une exception WavFileException est levée.
	 * @param fileName le nom du fichier
	 * @throws IOException ou WavFileException en cas d'erreur de lecture
	 */
	public void loadFromWavFile(String fileName) throws IOException, WavFileException{
		// Open the wav file specified as the first argument
		WavFile wavFile = WavFile.openWavFile(new File(fileName));

		if(wavFile.getSampleRate() != SAMPLERATE) {
			throw new WavFileException("The sample rate of the audio file is not 44100 Hz");
		}

		reset();

		// Display information about the wav file
		// wavFile.display();
		// Get the number of audio channels in the wav file
		int numChannels = wavFile.getNumChannels();
		// Create a buffer of 1000 frames
		double[][] buffer = new double[numChannels][1000];
		int framesRead;
		do{
			// Read frames into buffer
			framesRead = wavFile.readFrames(buffer, 1000);

			// Passage en mono
			for (int s=0 ; s<framesRead  ; s++)
			{
				double valueMono = 0;
				for (int c=0 ; c<numChannels ; c++)  {
					valueMono += buffer[c][s];
				}
				valueMono = valueMono / numChannels;
				addSample(valueMono);
			}
		}
		while (framesRead != 0);

		// Close the wavFile
		wavFile.close();
	}


	/** Enregistre le signal dans un fichier audio au format .wav mono nommé fileName. 
	 * La fréquence d'échantillonnage du fichier sera 44100 Hz.
	 * Le signal est normalisé avant sauvegarde @see normalize()
	 * @param fileName le nom du fichier
	 * @throws IOException ou WavFileException en cas d'erreur de lecture
	 */
	public void saveAudioDataToWavFileNormalized(String fileName) throws IOException, WavFileException {
		// Le package WavFile attend que les échantillons soient entre -1 et 1 en double
		double max = getMaxValAbs();
		double [] buffer = new double[data.size()];
		int i = 0;
		for (double value : data) {
			buffer[i] = value;
			if(max > 0) {
				buffer[i] = buffer[i] * 0.9 / max; // normalisation à 0.9
			}
			i++;
		}
		WavFile wavFile = WavFile.newWavFile(new File(fileName), 1, data.size(), 16, SAMPLERATE);
		wavFile.writeFrames(buffer, data.size());
		wavFile.close(); 
	}

}
