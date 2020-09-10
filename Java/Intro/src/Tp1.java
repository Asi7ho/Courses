public class Tp1 {
    public static void main(String [] arg) {
        Bouteille btl1;
        btl1 = new Bouteille("coca", 100);

        Bouteille btl2;
        btl2 = new Bouteille("whisky", 75, true);

        Verre v;
        v = new Verre(20);

        Bar bar;
        bar = new Bar("OMaitre", 5);


        System.out.println("\nOn crée les bouteils et le verre");
        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);

        System.out.println("\nOn crée le bar");
        System.out.println(bar);

        System.out.println("\nOn range les bouteilles dans le bar");
        bar.ranger(btl1, 0);
        bar.ranger(btl2, 1);
        System.out.println(bar);

        System.out.println("\nOn prend la bouteille de whisky");
        bar.prendre("whisky");
        System.out.println(bar);

        System.out.println("\nOn verse 5cl de whisky dans le verre");
        btl2.setQuantity(btl2.getQuantity() - btl2.verser(v, 5));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);

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


        System.out.println("\nOn essaye de verser 10cl de coca dans le verre");
        btl1.setQuantity(btl1.getQuantity() - btl1.verser(v, 10));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);

        System.out.println("\nOn ferme les bouteilles et on boit en 2 fois");
        btl1.fermer();
        btl2.fermer();
        v.setQuantity(v.getQuantity() - v.boire(10));
        System.out.println(v);
        v.setQuantity(v.getQuantity() - v.boire(10));

        System.out.println(v);
        System.out.println(btl1);
        System.out.println(btl2);

        System.out.println("\nOn range les bouteilles dans le bar");
        bar.ranger(btl1, 0);
        bar.ranger(btl2, 1);
        System.out.println(bar);

    }
}
