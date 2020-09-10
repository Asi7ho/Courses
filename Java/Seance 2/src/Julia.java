public class Julia {
    private Image im;
    private double maxModule;
    private int maxTermes;
    private Complexe c;

    public Julia(int taille, int maxModule, int maxTermes, Complexe c) {
        this.im = new Image(taille, taille);
        this.maxModule = maxModule;
        this.maxTermes = maxTermes;
        this.c = c;
        System.out.println("Objet crée");
        this.genererImage();
    }

    public int getNombreDeLigne() {
        return this.im.getNombreDeLigne();
    }

    public int getNombreDeColonne() {
        return this.im.getNombreDeColonne();
    }

    public void setC(Complexe c){
        this.c = c;
    }

    private int calculer(Complexe z0){
        Complexe z_n;
        z_n = new Complexe();
        int pas = 0;

        z_n.setReelle(z0.getReelle());
        z_n.setImaginaire(z0.getImaginaire());

        while (z_n.getRho() < maxModule && pas < maxTermes) {
            z_n = Complexe.add(Complexe.mult(z_n,z_n), c);
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
