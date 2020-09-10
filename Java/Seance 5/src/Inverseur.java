import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class Inverseur extends Element {
    private Element source;

    public Inverseur (String name, Element source) {
        super(name);
        if (source == null) {
            throw new IllegalArgumentException("La source ne doit pas etre nulle");
        }
        this.source = source;
    }

    @Override
    public Set<Entree> getEntrees() {
        return this.source.getEntrees();
    }

    @Override
    public String toString() {
        return "L'inverseur " + getName() + " possede " + getEntrees() + " comme sources";
    }

    @Override
    public boolean evaluer(Map<Entree, Boolean> environnement) {
        return !this.source.evaluer(environnement);
    }
}
