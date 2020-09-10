import java.util.HashMap;
import java.util.Map;

public class TestCircuit1 {
    public static void main(String[] args) {
		Entree e0 = new Entree("e0");
		Entree e1 = new Entree("e1");
		Entree e2 = new Entree("e2");
		PorteOu c = new PorteOu("c", e0, e1);
		PorteEt b = new PorteEt("b", e0, e1);
		PorteOu d = new PorteOu("d", e1, e2);
		Inverseur e = new Inverseur("e", b);
		Inverseur f = new Inverseur("f", d);
		PorteEt a = new PorteEt("a", c, e);

		Circuit circ = new Circuit();
		circ.addSortie(a);
		circ.addSortie(b);
		circ.addSortie(f);
		circ.addSortie(e2);

		System.out.println("\nListe des entrées du circuit :");

		for (Entree entree: circ.getEntrees()) {
	    	System.out.println("→ entrée « " + entree + " »");
		}

		System.out.println("\nTable de verite du circuit");
		Map<Entree, Boolean> environnement = new HashMap<Entree, Boolean>();
		environnement.put(e0, false);
		environnement.put(e1, false);
		environnement.put(e2, false);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, false);
		environnement.put(e1, false);
		environnement.put(e2, true);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, false);
		environnement.put(e1, true);
		environnement.put(e2, false);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, false);
		environnement.put(e1, true);
		environnement.put(e2, true);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, true);
		environnement.put(e1, false);
		environnement.put(e2, false);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, true);
		environnement.put(e1, false);
		environnement.put(e2, true);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, true);
		environnement.put(e1, true);
		environnement.put(e2, false);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

		environnement.put(e0, true);
		environnement.put(e1, true);
		environnement.put(e2, true);

		System.out.println(environnement.values() + " -> " + circ.dicoSortie(environnement).values());

    }
}
