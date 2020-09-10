import phelma.ImageToolkit;
import java.lang.reflect.Array;
import java.util.Arrays;

public class Image {

    private int nbl;
    private int nbc;
    private int[][] data;

    public Image(int nbl, int nbc) {
        this.nbl = nbl;
        this.nbc = nbc;
        this.data = new int[nbl][nbc];
        for (int i = 0; i < nbl; i++){
            for(int j = 0; j < nbc; j++) {
                data[i][j] = 0xFF000000;
            }
        }
    }

    public Image(Image im) {
        this.nbc = im.nbc;
        this.nbl = im.nbl;
        this.data = new int[this.nbl][this.nbc];
        for (int i = 0; i < this.nbl; i++){
            this.data[i] = Arrays.copyOf(im.data[i], im.data[i].length);
        }
    }

    public int getNombreDeLigne() {
        return nbl;
    }

    public int getNombreDeColonne() {
        return nbc;
    }

    public int getPixel(int i, int j) {
        return data[i][j];
    }

    public void setPixel(int i, int j, int valeur) {
        data[i][j] = valeur;
    }

    public void drawRect(int x, int y, int H, int L) {
        for (int i = x; i < (x + H); i++) {
            for (int j = y; j < (y + L); j++) {
                data[i][j] = 0xFFFFFFFF;
            }
        }
    }

    public void afficher() {
        ImageToolkit.displayPixelMatrix(data);
    }

    public void sauver(String nom) {
        ImageToolkit.savePixelMatrix(nom, data);
    }

}
