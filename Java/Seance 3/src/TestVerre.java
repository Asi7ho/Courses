public class TestVerre {
    public static void main(String [] arg) {
        Verre verre;
        verre = new Verre(0);

        System.out.println("\nOn crée un verre de contanance 20 cl");
        verre.setContenance(20);

        System.out.println("On met 10 cl dans le verre");
        verre.setQuantity(verre.remplir(10));
        System.out.println(verre);

        System.out.println("\nOn essaye de mettre 15 cl en plus dans le verre");
        verre.setQuantity((verre.remplir(15)));
        System.out.println(verre);

        System.out.println("\nOn boit 10 cl");
        verre.setQuantity(verre.getQuantity() - verre.boire(10));
        System.out.println(verre);

        System.out.println("\nOn essaye de boire 10 cl");
        verre.setQuantity(verre.getQuantity() - verre.boire(20));
        System.out.println(verre);

        System.out.println("\nOn change la contenance du verre");
        verre.setContenance(100);
        System.out.println(verre);

        System.out.println("\nOn change comment on crée l'objet verre");
        Verre verre2;
        verre2 = new Verre(20);
        System.out.println(verre2);
    }
}
