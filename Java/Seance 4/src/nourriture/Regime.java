package nourriture;

import zoo.animal.Animal;

public abstract class Regime extends Animal {

    double quantiteNourriture;



    public Regime (String nom, String regime, double quantiteNourriture, double poids, String cri) {
        super(nom, regime, poids, cri);
        this.quantiteNourriture = quantiteNourriture;
    }

    public Regime (String nom, String regime, double quantiteNourriture, double poids, String cri, Animal pere, Animal mere) {
        super(nom, regime, poids, cri, pere, mere);
        this.quantiteNourriture = quantiteNourriture;
    }

    protected void setQuantitéNourriture(double quantitéNourriture) {
        this.quantiteNourriture = quantiteNourriture;
    }

    public double getQuantitéNourriture() {
        return quantiteNourriture;
    }

    public abstract double getCout();
}
