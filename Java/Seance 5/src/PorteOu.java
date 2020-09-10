import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class PorteOu extends PorteBinaire {

    public PorteOu(String name, Element source1, Element source2) {
        super(name, source1, source2);
    }

    @Override
    public Set<Entree> getEntrees() {
        HashSet<Entree> entrees = new HashSet<Entree>();
        entrees.addAll(this.source1.getEntrees());
        entrees.addAll(this.source2.getEntrees());
        return entrees;
    }

    @Override
    public String toString() {
        return "La porte ou \"" + getName() + "\" possede " + getEntrees() + " comme sources";
    }

    @Override
    public boolean evaluer(Map<Entree, Boolean> environnement) {
        return this.source1.evaluer(environnement) || this.source2.evaluer(environnement);
    }
}
