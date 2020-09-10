package phelmaaudio;

import javax.swing.JFrame;
import javax.swing.*;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.text.NumberFormat;
import java.util.*;

public class WaveformFrame extends JFrame{
	private LinkedList<AudioData> dataList;
	LinkedList<WaveformPanelContainer> affList;

	private JFormattedTextField jtfMinY;
	private JFormattedTextField jtfMaxY;

	static int posx=0, posy=0;



	public WaveformFrame(AudioData data) {
		super();
		this.dataList = new LinkedList<AudioData>();
		dataList.add(data);

		createWindow();
	}

	public WaveformFrame(Collection<AudioData> dataList) {
		super();
		this.dataList = new LinkedList<AudioData>();
		for(AudioData data : dataList){
			this.dataList.add(data);
		}
		createWindow();
	}

	private void createWindow(){
		affList = new LinkedList<>();
		for(AudioData data : dataList){
			affList.add(new WaveformPanelContainer(data));
		}

		JLabel label1 = new JLabel("min Y");
		jtfMinY = new JFormattedTextField(NumberFormat.getNumberInstance());
		jtfMinY.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent e) {
				try{
					double valueMin = Double.parseDouble(jtfMinY.getText());
					for(WaveformPanelContainer aff : affList){
						aff.setMinY(valueMin);
					}
				} catch(NumberFormatException ex){
					System.out.println(ex);
				}
				//System.out.println(aff.getMinY());
				jtfMinY.setText(Double.toString(affList.get(0).getMinY())); // **TODO bof
			}
		}
				);

		JLabel label2 = new JLabel("max Y");
		jtfMaxY = new JFormattedTextField(NumberFormat.getNumberInstance());
		jtfMaxY.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent e) {
				try{
					double valueMax = Double.parseDouble(jtfMaxY.getText());
					for(WaveformPanelContainer aff : affList){
						aff.setMaxY(valueMax);
					}
				} catch(NumberFormatException ex){
					System.out.println(ex);
				}
				jtfMinY.setText(Double.toString(affList.get(0).getMaxY())); // **TODO bof
			}
		}
				);



		JLabel label3 = new JLabel("First Sample");
		JFormattedTextField jtfFirstSample = new JFormattedTextField(NumberFormat.getIntegerInstance());
		jtfFirstSample.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent e) {
				try{
					int value = Integer.parseInt(jtfFirstSample.getText());
					for(WaveformPanelContainer aff : affList){
						aff.setFirstSample(value);
					}
				} catch(NumberFormatException ex){
					System.out.println(ex);
				}
				jtfFirstSample.setText(Integer.toString(affList.get(0).getFirstSample())); // **TODO bof
			}
		}
				);

		JLabel label4 = new JLabel("Samples per pixel");
		JFormattedTextField jtfTimeIncrement = new JFormattedTextField(NumberFormat.getIntegerInstance());
		jtfTimeIncrement.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent e) {
				try{
					int value = Integer.parseInt(jtfTimeIncrement.getText());
					for(WaveformPanelContainer aff : affList){
						aff.setIncrement(value);
					}
				} catch(NumberFormatException ex){
					System.out.println(ex);
				}
				jtfTimeIncrement.setText(Integer.toString(affList.get(0).getIncrement())); // bof
			}
		}
				);

		jtfMinY.setText(Double.toString(affList.get(0).getMinY())); //**TODO bof : get min des min
		jtfMaxY.setText(Double.toString(affList.get(0).getMaxY())); //**TODO bof : get max des max
		jtfFirstSample.setText(Integer.toString(affList.get(0).getFirstSample()));  //**TODO bof : get first des first
		jtfTimeIncrement.setText(Integer.toString(affList.get(0).getIncrement()));//**TODO bof : get incr des incr

		JPanel controlPanel = new JPanel(new GridLayout(5,2));
		controlPanel.add(label1);
		controlPanel.add(jtfMinY);
		controlPanel.add(label2);
		controlPanel.add(jtfMaxY);
		controlPanel.add(Box.createVerticalStrut(10));
		controlPanel.add(Box.createVerticalStrut(10));
		controlPanel.add(label3);
		controlPanel.add(jtfFirstSample);
		controlPanel.add(label4);
		controlPanel.add(jtfTimeIncrement);

		JPanel waveformPanel = new JPanel(new GridLayout(0,1));

		for(WaveformPanelContainer aff : affList){
			waveformPanel.add(aff);
		}

		
		setLayout(new GridLayout(1,0));
		add(controlPanel);
		add(waveformPanel);

		//aff.setBorder(new LineBorder(Color.DARK_GRAY));
		setPreferredSize(new Dimension(1000, 200)); 
		pack();
		setLocation(posx, posy);
		posy+=100;
		posy+=20;
	}
}
