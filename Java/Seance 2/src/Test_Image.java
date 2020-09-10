public class Test_Image {
    public static void main(String [] arg) {
        Image im1;
        im1 = new Image(500, 500);

        im1.drawRect(0, 0, 100, 100);

        //im1.afficher();

        im1.sauver("test");


        Image im2;
        im2 = new Image(im1);

        im2.drawRect(100, 100, 100, 100);
        im2.afficher();

        im1.afficher();

        im2.sauver("test2");

    }
}
