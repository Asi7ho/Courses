package zoo.animal;

import nourriture.Viande;

public class TigreBlanc extends Viande {
    private int nombreDeRayures;

    public TigreBlanc(String nom, double poids, int nombreDeRayures) {
        super(nom, 4, poids, "feulement");
        this.nombreDeRayures = nombreDeRayures;
    }

    public void setNombreDeRayures(int nombreDeRayures) {
        this.nombreDeRayures = nombreDeRayures;
    }

    public int getNombreDeRayures() {
        return nombreDeRayures;
    }

    @Override
    public String toString() {
        return "Le tigre blanc" + getNom() + " mange " + getQuantitéNourriture() + "kg, a " + nombreDeRayures + " et son cri est un " + getCri() + " et il coute : " + getCout();
    }
}
