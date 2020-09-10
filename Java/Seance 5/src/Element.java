import java.util.Map;
import java.util.Set;

public abstract class Element {
    private String name;

    public Element(String name) {
        if (name == null || name.equals("")) {
                throw new IllegalArgumentException("Veuillez mettre un nom");
        }

        this.name = name;
    }


    public String getName() {
        return name;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Element){
            return this.getName().equals(((Element)obj).getName());
        } else if (obj instanceof String) {
            return this.getName().equals((String)obj);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return this.getName().hashCode();
    }

    public abstract Set<Entree> getEntrees();

    @Override
    public String toString() {
        return "L'element est un " + name;
    }

    public abstract boolean evaluer(Map<Entree, Boolean> environnement);
}
