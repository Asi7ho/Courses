package nourriture;

import zoo.animal.Animal;

public class Legume extends Regime {

    public Legume(String nom, double quantiteNourriture, double poids, String cri) {
        super(nom, "Végétarien", quantiteNourriture, poids, cri);
    }

    public Legume(String nom, double quantiteNourriture, double poids, String cri, Animal pere, Animal mere) {
        super(nom, "Végétarien", quantiteNourriture, poids, cri, pere, mere);
    }

    @Override
    public double getCout() {
        return 1.2*Math.log((getQuantitéNourriture() + 5) * 2 + 1);
    }
}
