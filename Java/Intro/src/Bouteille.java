public class Bouteille {
    //attribut
    private String nom;
    private int quantite;
    private boolean ouverte;

    //constructeur
    public Bouteille(String nom, int q, boolean ouverte){
        this.nom = nom;
        this.quantite = q;
        this.ouverte = ouverte;
    }

    public Bouteille(String nom, int q){
        this.nom = nom;
        this.quantite = q;
        this.ouverte = false;
    }

    //méthodes
    public String getNom(){
        return nom;
    }

    public int getQuantity() {
        return this.quantite;
    }

    public void setQuantity(int nb) {
        if (nb < 0) {
            throw new IllegalArgumentException("la quantité doit est positive !");
        }
        this.quantite = nb;
    }

    public void ouvrir() {
        ouverte = true;
    }

    public void fermer() {
        ouverte = false;
    }

    public void setOuverte() {
        if (this.ouverte == false) {
            throw new IllegalArgumentException("la bouteille doit etre ouverte !");
        }
    }

    public int verser(int q) {
        setOuverte();
        int newQuantity = quantite - q;
        if (newQuantity > 0) {
            return q;
        } else {
            return quantite;
        }
    }

    public int verser(Verre v, int q) {
        setOuverte();
        int newQuantity = v.getContenance() - q;
        if (newQuantity < 0) {
            v.setQuantity(v.remplir(v.getContenance()));
            return v.getContenance();
        } else if (v.getContenance() == v.getQuantity()){
            return 0;
        } else {
            v.setQuantity(v.remplir(q));
            return q;
        }
    }

    public String toString() {
        return new String("La bouteille de " + nom + " contient " + quantite + "cl");
    }

}
