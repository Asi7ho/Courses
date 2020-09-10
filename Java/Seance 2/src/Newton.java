public class Newton {
    private Image im;
    private double precision;
    private int ordre;
    private int maxTermes;

    public Newton(int taille, int maxTermes, double precision, int ordre) {
        this.im = new Image(taille, taille);
        this.precision = precision;
        this.ordre = ordre;
        this.maxTermes = maxTermes;
        System.out.println("Objet créée");
        this.genererImage();
    }

    public int getNombreDeLigne() {
        return this.im.getNombreDeLigne();
    }

    public int getNombreDeColonne() {
        return this.im.getNombreDeColonne();
    }

    private int calculer(Complexe z0){
        Complexe z_n = new Complexe();
        Complexe unite = new Complexe(1, 0);
        Complexe coeff = new Complexe((double)(ordre - 1) / ordre, 0);
        int pas = 0;

        z_n.setReelle(z0.getReelle());
        z_n.setImaginaire(z0.getImaginaire());

        while (Complexe.sub(Complexe.mult(coeff, Complexe.divide(Complexe.add(Complexe.pow(z_n, ordre), unite), Complexe.pow(z_n, ordre - 1))), z_n).getRho() >= precision && pas < maxTermes) {
            z_n = Complexe.mult(coeff, Complexe.divide(Complexe.add(Complexe.pow(z_n, ordre), unite), Complexe.pow(z_n, ordre - 1)));
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
