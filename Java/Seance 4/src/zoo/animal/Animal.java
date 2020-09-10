package zoo.animal;

import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;

public abstract class Animal {
    private String nom;
    private String type;
    private String cri;
    private double poids;
    private double cout;
    private Animal pere;
    private Animal mere;
    private HashSet<Animal> enfant = new HashSet<Animal>();


    public Animal(String nom, String type, double poids,String cri) {
        this.nom = nom;
        this.type = type;
        this.poids = poids;
        this.cri = cri;
        this.cout = 0;
        this.pere = null;
        this.mere = null;
    }

    public Animal(String nom, String type, double poids,String cri, Animal pere, Animal mere) {
        this.nom = nom;
        this.type = type;
        this.poids = poids;
        this.cri = cri;
        this.cout = 0;
        this.pere = pere;
        this.pere.enfant.add(this);
        this.mere = mere;
        this.mere.enfant.add(this);
    }

    @Override
    public int hashCode() {
        return this.nom.hashCode();
    }

    public String getNom() {
        return nom;
    }

    public String getType() {
        return type;
    }

    protected void setPoids(double poids) { this.poids = poids; }

    public double getPoids() { return poids; }

    public String getCri() { return cri; }

    public abstract double getCout();

    public int getDescendant() {
        int nbrDescendance = this.enfant.size();
        Iterator<Animal> iterator = this.enfant.iterator();
        while(iterator.hasNext()){
            Animal a = iterator.next();
            nbrDescendance += a.enfant.size();
        }
        return nbrDescendance;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Animal){
            return this.getNom().equals(((Animal)obj).getNom());
        } else if (obj instanceof String) {
            return this.getNom().equals((String)obj);
        }
        return false;
    }

    public String toString() {
        return new String("L'Animal s'appelle " + nom + ", il est de type " + type + " son cri est un " + cri + " et il coute : " + cout);
    }


}
