package zoo.animal;

import nourriture.Viande;

public class Orque extends Viande {

    public Orque(String nom, double poids) {
        super(nom, 0.1*poids, poids, "sifflement");
    }

    @Override
    public void setPoids(double poids) {
        setQuantitéNourriture(0.1*poids);
        super.setPoids(poids);
    }

    @Override
    public String toString() {
        return "L'orque " + getNom() + " mange " + getQuantitéNourriture() + "kg et son cri est un " + getCri() + " et il coute : " + getCout();
    }
}
