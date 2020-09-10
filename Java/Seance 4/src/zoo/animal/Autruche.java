package zoo.animal;

import nourriture.Legume;

public class Autruche extends Legume{
    private int nombreDePlume;

    public Autruche(String nom, double poids, int nombreDePlume) {
        super(nom, 5, poids, "beuglement");
        this.nombreDePlume = nombreDePlume;
    }

    public void setNombreDePlume(int nombreDePlume) {
        this.nombreDePlume = nombreDePlume;
    }

    public int getNombreDePlume() {
        return nombreDePlume;
    }

    @Override
    public String toString() {
        return "L'autruche " + getNom() + " mange " + getQuantitéNourriture() + "kg/jour, a " + nombreDePlume + " plumes et son cri est un " + getCri() + " et il coute : " + getCout();
    }
}
