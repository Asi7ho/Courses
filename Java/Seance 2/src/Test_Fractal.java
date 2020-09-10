public class Test_Fractal {
    public static void main(String [] arg) {
        Mandel m;
        m = new Mandel(300, 10, 100);

        m.afficher();
        m.sauver("mandela");

        Complexe c;
        c = new Complexe(-0.5, 0.5);
        Julia j;
        j = new Julia(300, 10, 100, c);

        j.afficher();;
        j.sauver("julia");

        Newton n;
        n = new Newton(300, 10, 0.00001, 5);

        n.afficher();
        n.sauver("newton");
    }
}
