import java.time.Duration;
import java.time.LocalDateTime;

public class BouteilleJus extends Bouteille {
    private  int secondsAvantPeremption;
    private LocalDateTime dateTimeOuverture;

    public BouteilleJus(String nom, int q) {
        super(nom, q, false);
        this.secondsAvantPeremption = 1;
    }

    public BouteilleJus(String nom, int q, int secondsAvantPeremption) {
        super(nom, q, false);
        this.secondsAvantPeremption = secondsAvantPeremption;
    }

    @Override
    public void ouvrir() {
        dateTimeOuverture = LocalDateTime.now();
        super.ouvrir();
    }

    @Override
    public int verser(Verre v, int q) {
        if (estPerimee()) {
            System.out.println("A vos risques et peril");
        }
        return super.verser(v, q);
    }

    public boolean estPerimee() {
        long time = Duration.between(dateTimeOuverture, LocalDateTime.now()).toMillis()/1000;
        if (time > secondsAvantPeremption) {
            System.out.println("Bouteille est périmé depuis " + (time - secondsAvantPeremption) + " secondes");
            return true;
        } else {
            return false;
        }
    }
}
