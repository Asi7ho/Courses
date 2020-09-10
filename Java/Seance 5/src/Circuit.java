import java.util.*;

public class Circuit {
    private Set<Element> sorties;

    public Circuit(){
        this.sorties = new HashSet<Element>();
    }

    public void addSortie(Element e) {
        sorties.add(e);
    }

    public Set<Entree> getEntrees() {
        HashSet<Entree> entrees = new HashSet<Entree>();
        Iterator<Element> iterator = this.sorties.iterator();
        while(iterator.hasNext()){
            Element e = iterator.next();
            entrees.addAll(e.getEntrees());
        }
        return entrees;
    }

    public Map<Element, Boolean> dicoSortie(Map<Entree, Boolean> environnement) {
        HashMap<Element, Boolean> operation = new HashMap<Element, Boolean>();
        Iterator<Element> iterator = this.sorties.iterator();
        while (iterator.hasNext()) {
            Element e = iterator.next();
            operation.put(e, e.evaluer(environnement));
        }
        return operation;
    }

}
