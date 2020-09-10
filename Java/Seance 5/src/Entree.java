import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class Entree extends Element {

    public Entree(String name) {
        super(name);
    }

    @Override
    public Set<Entree> getEntrees() {
        HashSet<Entree> entrees = new HashSet<Entree>();
        entrees.add(this);
        return entrees;
    }

    @Override
    public String toString() {
        return getName();
    }

    @Override
    public boolean evaluer(Map<Entree, Boolean> environnement) {
        if (environnement.containsKey(this)) {
            return environnement.get(this);
        } else {
            throw new IllegalArgumentException("L'entree n'est pas dans l'environnement");
        }
    }
}
