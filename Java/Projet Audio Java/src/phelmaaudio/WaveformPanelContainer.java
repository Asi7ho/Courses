package phelmaaudio;



import javax.swing.*;
//import javax.sound.sampled.AudioInputStream;
import java.awt.*;
import java.util.ArrayList;



/** D'apres Maciej Ciemborowicz, projet VirtualCut
 * https://github.com/ciembor
 * https://github.com/ciembor/VirtualCut
 * Adaptation NC, fevrier 2016
 */
public class WaveformPanelContainer extends JPanel {
	// Les waveform, 1 par canal (une seule pour le moment)
	private ArrayList<SingleWaveformPanel> singleChannelWaveformPanels ;
	private AudioData data = null;

	// ZOOM : commun à toutes les SingleWaveformPanel dans notre cas
	// (d'ailleurs, il n'y en a qu'une dans notre cas pour le moment)
	// Zoom temps
	private double minY;
	private double maxY;
	// Zoom temps
	private int firstSample; // premien sample a gauche
	private int increment; // nb de samples par pixel

	public WaveformPanelContainer(AudioData data){
		this.data = data;
		singleChannelWaveformPanels = new ArrayList<SingleWaveformPanel>();
		setMinY(-1);
		setMaxY(-1);
		setFirstSample(0);
		setIncrement(1);
		setSize();
		setAudioToDisplay(data);
	}

	// Point de vue Y (sample value)
	double getMinY() {
		return minY;
	}
	double getMaxY() {
		return maxY;
	}
	void setMinY(double minY) {
		if(minY >= maxY) {
			return;
		}
		this.minY=minY;
		for(SingleWaveformPanel panel : singleChannelWaveformPanels) {
			panel.setMinY(minY);
		}
	}
	void setMaxY(double maxY) {
		if(minY >= maxY) {
			return;
		}
		this.maxY=maxY;
		for(SingleWaveformPanel panel : singleChannelWaveformPanels) {
			panel.setMaxY(maxY);
		}
	}


	void resetMinMaxY() {
		double min = data.getMin();
		double max = data.getMax();
		if(min == 0 && max == 0.){
			setMinY(-1);
			setMaxY(+1);
		} else {
			setMinY(min*1.1);
			setMaxY(max*1.1);
		}
	}

	// POint de vue X (temps)
	int getFirstSample() {
		return firstSample;
	}
	int getIncrement() {
		return increment;
	}
	void setFirstSample(int v) {
		if(v < 0 ) {
			v = 0;
		}
		this.firstSample=v;
		for(SingleWaveformPanel panel : singleChannelWaveformPanels) {
			panel.setFirstSample(v);
		}
	}
	void setIncrement(int nb) {
		if(nb <=0 ) {
			nb = 1;
		}
		this.increment=nb;
		for(SingleWaveformPanel panel : singleChannelWaveformPanels) {
			panel.setIncrement(nb);
		}
		repaint();
	}

	public void setAudioToDisplay(AudioData data){
		this.data = data;
		this.singleChannelWaveformPanels.clear();
		setLayout(new GridLayout(1, 0));

		//for (int t=0; t<trackModel.getNumberOfChannels(); t++){
		// 1 seule waveform pour le moment. NC fevrier 2016
		for (int t=0; t< 1; t++){ 
			SingleWaveformPanel waveformPanel
			= new SingleWaveformPanel(data);
			//= new SingleWaveformPanel(data, t);
			singleChannelWaveformPanels.add(waveformPanel);
			JComponent panel = createChannelDisplay(waveformPanel, t);
			add(panel);
			setSize();

			// reset zoom
			resetMinMaxY();
			setFirstSample(0);
			setIncrement(1);
		}
	}

	private void setSize() {
		//this.setSize(trackModel.getSize());
		this.setSize( new Dimension(2000, 100));
	}

	private JComponent createChannelDisplay(SingleWaveformPanel waveformPanel, int index) {
		JPanel panel = new JPanel(new BorderLayout());
		panel.add(waveformPanel, BorderLayout.CENTER);
		
		JButton b = new JButton("play");
		b.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent e) {
				try{
					AudioDataPlayer p = new AudioDataPlayer(data);
					p.start();
				} catch(Exception exception) {
					System.out.println("Cannot play. Exception " + exception);
				}
			}
		});
		b.setPreferredSize(new Dimension( 50, 10));;
		panel.add(b, BorderLayout.LINE_END);

		// JLabel label = new JLabel("Channel " + ++index);
		// panel.add(label, BorderLayout.NORTH);

		return panel;
	}


}


