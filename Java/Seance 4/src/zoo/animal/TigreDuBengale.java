package zoo.animal;

import nourriture.Viande;

public class TigreDuBengale extends Viande {
    private int nombreDeRayures;

    public TigreDuBengale(String nom, double poids, int nombreDeRayures) {
        super(nom, 4, poids,"feulement");
        this.nombreDeRayures = nombreDeRayures;
    }

    public TigreDuBengale(String nom, double poids, int nombreDeRayures, Animal pere, Animal mere) {
        super(nom, 4, poids,"feulement", pere, mere);
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
        return "Le tigre du Bengale " + getNom() + " mange " + getQuantitéNourriture() + "kg, a " + nombreDeRayures + " et son cri est un " + getCri() + " et il coute : " + getCout();
    }
}
