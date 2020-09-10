package phelma;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;
//import javax.swing.border.*;
//import java.awt.*;


public class ImageToolkit {
	static int posx=0, posy=0;
	
	public static void displayPixelMatrix(int [][] pixelMatrix) {
		int nbl = pixelMatrix.length;
		int nbc = pixelMatrix[0].length;
		
		BufferedImage bitmap= new BufferedImage(nbc, nbl, BufferedImage.TYPE_INT_ARGB);
		for(int i = 0 ; i< nbc ; i ++) {
			for(int j = 0 ; j < nbl ; j ++){
				bitmap.setRGB(i, j, pixelMatrix[j][i]);
			}
		}		
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		JPanel aff = new JPanel() { 
			public void paint(Graphics g2) { 
					g2.drawImage(bitmap, 0, 0, null);
			}
		}; 
		//aff.setBorder(new LineBorder(Color.DARK_GRAY));
		aff.setPreferredSize(new Dimension(nbc, nbl)); 
		frame.add(aff);
		frame.pack();
		if(posx+ nbc > Toolkit.getDefaultToolkit().getScreenSize().getWidth()) {
			posx = 0;
			posy += nbl + 100;
		}
		frame.setLocation(posx, posy);
		posx+=nbc+10;
		frame.setVisible(true); 
	}
	
	public static boolean savePixelMatrix(String fileName, int [][] pixelMatrix)  {
		int nbl = pixelMatrix.length;
		int nbc = pixelMatrix[0].length;
	    BufferedImage bitmap = new BufferedImage(nbc,nbl,BufferedImage.TYPE_INT_RGB);
		for(int i = 0 ; i< nbc ; i ++) {
			for(int j = 0 ; j < nbl ; j ++){
				bitmap.setRGB(i, j, pixelMatrix[j][i]);
			}
		}	
		
		try {
	    	ImageIO.write(bitmap, "PNG", new File(fileName +".png")); 
	    } catch( IOException e){
	    	System.out.println("Erreru ecruiture fichier"); 
	    	return false;
	    }
		return true;
	  }

}
