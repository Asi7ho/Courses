import phelma.ImageToolkit;

public class Mandel {
    private Image im;
    private double maxModule;
    private int maxTermes;

    public Mandel(int taille, int maxModule, int maxTermes) {
        this.im = new Image(taille, taille);
        this.maxModule = maxModule;
        this.maxTermes = maxTermes;
        System.out.println("Objet crée");
        this.genererImage();
    }

    public int getNombreDeLigne() {
        return this.im.getNombreDeLigne();
    }

    public int getNombreDeColonne() {
        return this.im.getNombreDeColonne();
    }

    private int calculer(Complexe zp){
        Complexe z_n;
        z_n = new Complexe();
        int pas = 0;

        z_n.setReelle(0);
        z_n.setImaginaire(0);

        while (z_n.getRho() < maxModule && pas < maxTermes) {
            z_n = Complexe.add(Complexe.mult(z_n,z_n), zp);
            pas++;
            //System.out.println(pas);
        }
        return pas;
    }

    private void genererImage() {
        Complexe zp;
        zp = new Complexe();
        for (int i = 0; i < im.getNombreDeLigne(); i++) {
            for (int j = 0; j < im.getNombreDeColonne(); j++) {

                zp.setReelle(-1 + (2.0*i)/im.getNombreDeLigne());
                zp.setImaginaire(-1 + (2.0*j)/im.getNombreDeColonne());

                if (calculer(zp) == maxTermes) {
                    im.setPixel(i, j, 0xFF000000);
                } else {
                    im.setPixel(i, j, 0xFF00FFFF);
                }
            }
        }
        System.out.println("image genérée");
    }

    public void afficher() {
        this.im.afficher();
    }

    public void sauver(String nom) {
        this.im.sauver(nom);
    }



}
