package phelmaaudio;

import javax.swing.*;
import java.awt.*;


/** D'apres Maciej Ciemborowicz, projet VirtualCut
 * https://github.com/ciembor
 * https://github.com/ciembor/VirtualCut
 * Adaptation NC, fevrier 2016
 */
public class SingleWaveformPanel extends JPanel {
	protected static final Color BACKGROUND_COLOR = Color.black;
	protected static final Color REFERENCE_LINE_COLOR = Color.gray;
	protected static final Color WAVEFORM_COLOR = Color.orange;

	private AudioData data;
	//    private int channelIndex;
	private double width;

	// Zoom temps
	private double minY;
	private double maxY;
	// Zoom temps
	private int firstSample; // premien sample a gauche
	private int increment; // nb de samples par pixel

	// Cas ou il y aurait plusieurs channel dans un audio data. Un seul pour le moment !
	//    public SingleWaveformPanel(AudioData data, int channelIndex) {
	//        this.data = data;
	//        this.channelIndex = channelIndex;
	//        this.minY = -1;
	//        this.maxY = 1;
	//        
	//        setBackground(BACKGROUND_COLOR);
	//        //width = model.getWidth() * model.getXScaleFactor(model.getWidth());
	//        width = 100*10;
	//        setPreferredSize(new Dimension((int)width, 140));
	//    }

	public SingleWaveformPanel(AudioData data) {
		this.data = data;
		//        this.channelIndex = channelIndex;
		this.minY = -1;
		this.maxY = 1;
		firstSample = 0;
		increment = 1;

		setBackground(BACKGROUND_COLOR);
		//width = model.getWidth() * model.getXScaleFactor(model.getWidth());
		width = 100*10;
		setPreferredSize(new Dimension((int)width, 140));
	}


	double getMinY() {
		return maxY;
	}
	double getMaxY() {
		return maxY;
	}
	void setMinY(double minY) {
		if(minY >= maxY) {
			return;
		}
		this.minY=minY;
		repaint();
	}
	void setMaxY(double maxY) {
		if(minY >= maxY) {
			return;
		}
		this.maxY=maxY;
		repaint();
	}

	int getFirstSample() {
		return firstSample;
	}
	int getIncrement() {
		return increment;
	}
	void setFirstSample(int v) {
		this.firstSample=v;
		repaint();
	}
	void setIncrement(int nb) {
		if(nb <=0 ) {
			return;
		}
		this.increment=nb;
		repaint();
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);


		drawWaveform(g);
	}

	protected void drawWaveform(Graphics g) {
		if (data == null) {
			return;
		}

		// la ligne horizontal marquant la valeur 0
		g.setColor(REFERENCE_LINE_COLOR);
		double scaledOfZeroSampleValue = (0. - minY) / (maxY - minY);
		int yOfZeroSampleValue = getHeight()-(int) ((getHeight() * scaledOfZeroSampleValue));
		g.drawLine(0, yOfZeroSampleValue, (int)getWidth(), yOfZeroSampleValue);

		g.setColor(WAVEFORM_COLOR);
		for (int pixel = 0; pixel < getWidth(); pixel++) {
			int sampleId = firstSample + pixel * increment;
			if(sampleId >= data.getNbSamples()) {
				g.setColor(new Color(230, 230, 230, 200));
				g.fillRect(pixel, 0, getWidth() - pixel, getHeight());
				return;
			}
			double minValue = data.getSample(sampleId);
			double maxValue = data.getSample(sampleId);
			sampleId++;
			while(sampleId < firstSample + (pixel+1) * increment
					&& sampleId < data.getNbSamples()) {
				minValue = Math.min(minValue, data.getSample(sampleId));
				maxValue = Math.max(maxValue, data.getSample(sampleId));
				sampleId ++;
			}
			double scaledMinValue = (minValue - minY) / (maxY - minY);
			double scaledMaxValue = (maxValue - minY) / (maxY - minY);

			int minY = getHeight()-(int) ((getHeight() * scaledMinValue));
			int maxY = getHeight()-(int) ((getHeight() * scaledMaxValue));
			g.drawLine(pixel, minY, pixel, maxY);
			
			//TODO améliorer l'affichage dans le cas "échantillons disjoints"
		}
	}
}
