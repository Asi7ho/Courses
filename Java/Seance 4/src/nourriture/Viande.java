package nourriture;

import zoo.animal.Animal;

public class Viande extends Regime{
    public Viande(String nom, double quantiteNourriture, double poids, String cri) {
        super(nom, "Carnivore", quantiteNourriture, poids, cri);
    }

    public Viande(String nom, double quantiteNourriture, double poids, String cri, Animal pere, Animal mere) {
        super(nom, "Carnivore", quantiteNourriture, poids, cri, pere, mere);
    }

    @Override
    public double getCout() {
        return Math.pow(10*quantiteNourriture, 2) + 100;
    }
}
