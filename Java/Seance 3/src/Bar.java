import javax.lang.model.type.NullType;
import java.lang.reflect.Array;

public class Bar {
    private String nom;
    private Bouteille[] tab;

    public Bar(String nom, int nbEmplcement){
        this.nom = nom;
        tab = new Bouteille[nbEmplcement];
    }

    public String getNom() {
        return nom;
    }

    public int getNbEmplacement() {
        return tab.length;
    }

    public void ranger(Bouteille b, int numEmplacement) {
        if (tab[numEmplacement] != null) {
            throw new IllegalArgumentException("la place doit etre libre !");
        }
        tab[numEmplacement] = b;
    }

    public Bouteille prendre(int numEmplacement) {
        Bouteille btl;
        if (tab[numEmplacement] == null) {
            return null;
        } else {
            btl = tab[numEmplacement];
            tab[numEmplacement] = null;
            return btl;
        }
    }

    public Bouteille prendre(String nom) {
        Bouteille b;
        for (int i = 0; i < tab.length; i++) {

            if (tab[i] == null) {
                i += 1;
            }

            if (tab[i].getNom().equals(nom)) {
                b = tab[i];
                tab[i] = null;
                return b;
            }
        }
        return null;
    }

    public String toString() {
        String s = new String ();
        for (int i = 0; i < tab.length; i++) {
            s += tab[i] + "\n";
        }
        return new String("Le bar contient " + tab.length + " emplacement(s) : \n" + s);

    }





}
