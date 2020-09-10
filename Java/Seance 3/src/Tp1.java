public class Tp1 {
    public static void main(String [] arg) {
        Bouteille btl1;
        btl1 = new Bouteille("coca", 100);

        Bouteille btl2;
        btl2 = new Bouteille("whisky", 75, true);

        BouteilleJus btl3;
        btl3 = new BouteilleJus("Jus de Mangue", 100);

        Bouteille btl4;
        btl4 = new Bouteille("Pastis", 100);

        Verre v;
        v = new Verre(30);

        Bar bar;
        bar = new Bar("OMaitre", 5);


        System.out.println("\nOn crée les bouteils et le verre");
        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn crée le bar");
        System.out.println(bar);

        System.out.println("\nOn range les bouteilles dans le bar");
        bar.ranger(btl1, 0);
        bar.ranger(btl2, 1);
        bar.ranger(btl3, 2);
        bar.ranger(btl4, 3);
        System.out.println(bar);

        System.out.println("\nOn prend la bouteille de whisky");
        bar.prendre("whisky");
        System.out.println(bar);

        System.out.println("\nOn verse 5cl de whisky dans le verre");
        btl2.setQuantity(btl2.getQuantity() - btl2.verser(v, 5));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn prend la bouteille de coca");
        bar.prendre(0);
        System.out.println(bar);

        System.out.println("\nOn verse 15cl de coca dans le verre");
        try {
            btl1.setQuantity(btl1.getQuantity() - btl1.verser(v, 15));
        } catch (IllegalArgumentException e) {
            System.out.println("On ouvre la bouteille d'abord");
            btl1.ouvrir();
            btl1.setQuantity(btl1.getQuantity() - btl1.verser(v, 15));
        }

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);


        System.out.println("\nOn essaye de verser 10cl de coca dans le verre");
        btl1.setQuantity(btl1.getQuantity() - btl1.verser(v, 10));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn ferme les bouteilles et on boit en 2 fois");
        btl1.fermer();
        btl2.fermer();
        v.setQuantity(v.getQuantity() - v.boire(10));
        System.out.println(v);
        v.setQuantity(v.getQuantity() - v.boire(20));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn range les bouteilles dans le bar");
        bar.ranger(btl1, 0);
        bar.ranger(btl2, 1);
        System.out.println(bar);

        System.out.println("\nOn prend la bouteille de jus et la bouteille de Pastis");
        bar.prendre("Jus de Mangue");
        bar.prendre("Pastis");
        System.out.println(bar);

        System.out.println("\nOn ouvre les bouteilles");
        btl3.ouvrir();
        btl4.ouvrir();
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn attend 4 secondes");
        try {
            Thread.sleep(4000);
        } catch (InterruptedException e) {

        }
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn fais un cocktail");
        btl4.setQuantity(btl4.getQuantity() - btl4.verser(v, 10));
        System.out.println(v);
        btl3.setQuantity(btl3.getQuantity() - btl3.verser(v, 20));
        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);
        System.out.println(btl3);
        System.out.println(btl4);

        System.out.println("\nOn boit");
        v.setQuantity(v.getQuantity() - v.boire(30));
        System.out.println(v);



    }
}
