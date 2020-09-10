package zoo.animal;

import nourriture.Viande;

public class Aigle extends Viande {
    private double envergure;

    public Aigle(String nom, double poids, double envergure) {
        super(nom, 1, poids, "sifflement");
        this.envergure = envergure;
    }

    public Aigle(String nom, double envergure, double poids, int quantiteNourriture) {
        super(nom, quantiteNourriture, poids, "sifflement");
        this.envergure = envergure;
    }

    @Override
    public String toString() {
        return "L'aigle " + getNom() + " mange " + getQuantitéNourriture() + "kg/jour a une envergure de " + envergure + "m et son cri est un " + getCri() + " et il coute : " + getCout();
    }
}
