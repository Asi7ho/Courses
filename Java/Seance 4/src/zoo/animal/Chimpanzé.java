package zoo.animal;

import nourriture.Legume;

public class Chimpanzé extends Legume {
    public Chimpanzé(String nom, double poids) {
        super(nom, 3, poids, "hurlements");
    }

    @Override
    public String toString() {
        return "Le chimpanzé " + getNom() + " mange " + getQuantitéNourriture() + "kg/jour et son cri est un " + getCri() + " et il coute : " + getCout();
    }

}
