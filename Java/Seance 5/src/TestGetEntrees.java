public class TestGetEntrees {
    public static void main(String [] arg) {
        Entree entree1 = new Entree("b");
        Entree entree2 = new Entree("c");
        PorteEt et = new PorteEt("a", entree1, entree2);

        System.out.println(et);
    }
}
