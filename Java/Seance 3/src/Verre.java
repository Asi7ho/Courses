public class Verre {
    //attributs
    private int contenance;
    private int quantite;

    //constructeur
    public Verre(int c) {
        this.contenance = c;
    }

    //méthodes
    public int getVolumeVide() {
        return contenance - quantite;
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

    public int getContenance() {
        return this.contenance;
    }

    public void setContenance(int nb) {
        if (nb < 0) {
            throw new IllegalArgumentException("la contenance doit est positive !");
        }
        this.contenance = nb;
    }

    public int remplir(int q) {
        int newQuantity = quantite + q;
        if (newQuantity < contenance) {
            return newQuantity;
        } else {
            return contenance;
        }
    }

    public int boire(int q) {
        int newQuantity = quantite - q;
        if (newQuantity > 0) {
            return q;
        } else {
            return quantite;
        }
    }

    public String toString(){
        return new String("Le verre de " + contenance + "cl contient " + quantite + "cl");
    }

}
