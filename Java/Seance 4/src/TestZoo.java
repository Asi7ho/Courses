import zoo.Zoo;
import zoo.animal.*;

public class TestZoo {
    public static void main(String [] arg) {
        /*Aigle Francois = new Aigle("François", 1, 3);
        Chimpanzé Florent = new Chimpanzé("Florent", 30);
        Autruche Nicloas = new Autruche("Nicolas", 70, 100);


        System.out.println(Francois);
        System.out.println(Florent);
        System.out.println(Nicloas);*/

        Zoo Minatec = new Zoo("Minatec");

        /*TigreDuBengale Fantom = new TigreDuBengale("Fantom", 120, 40);
        Autruche Anne = new Autruche("Anne", 50, 100);
        Chimpanzé Chita = new Chimpanzé("Chita", 30);
        Aigle Roquette = new Aigle("Roquette", 0.2, 5, 2);
        Orque Azogue = new Orque("Azogue", 9000);

        Minatec.ajoute(Fantom);
        Minatec.ajoute(Anne);
        Minatec.ajoute(Chita);
        Minatec.ajoute(Roquette);
        Minatec.ajoute(Azogue);*/

        /*TigreDuBengale Fantom2 = new TigreDuBengale("Fantom", 120, 40);
        Minatec.ajoute(Fantom2);

        System.out.println(Minatec.recupereAnimal("Roquette"));

        Minatec.supprime("Chita");*/

        TigreDuBengale Fantom = new TigreDuBengale("Fantom", 120, 40);
        Minatec.ajoute(Fantom);
        TigreDuBengale Fantomette = new TigreDuBengale("Fantomette", 100, 20);
        Minatec.ajoute(Fantomette);
        TigreDuBengale Fantomas = new TigreDuBengale("Fantomas", 80, 20, Fantom, Fantomette);
        Minatec.ajoute(Fantomas);
        TigreDuBengale MiniGeignarde = new TigreDuBengale("Mini Geignarde", 80, 20, Fantom, Fantomette);
        Minatec.ajoute(MiniGeignarde);
        TigreDuBengale Casper = new TigreDuBengale("Casper", 80, 15, Fantomas, Fantomette);
        Minatec.ajoute(Casper);
        TigreDuBengale Georges = new TigreDuBengale("Georges", 80, 15, Casper, Fantomette);
        Minatec.ajoute(Georges);

        System.out.println("\nFantom a "+ Fantom.getDescendant() + " descandants\n");

        System.out.println(Minatec);
        System.out.println("Le cout total en nourriture pour le zoo est " + Minatec.coutTotal());
    }
}
