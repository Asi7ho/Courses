import java.util.HashMap;
import java.util.Map;

/**
 * Cette classe possède un certain nombre de méthodes
 * utilitaires permettant de réaliser un circuit booléen
 * additionneur de nombres positifs.
 *
 * @author Sylvain B.
 * @version 1.0
 */
public class Additionneur {
    private static int nbElements = 0;

    /**
     * Cette méthode réalise l'addition de deux entiers
     * en s'appuyant sur un circuit booléen. Si les classes
     * codant les éléments du circuit booléen sont correctes,
     * alors le comportement de cette méthode est celui attendu,
     * à savoir que l'entier renvoyé est le résultat de
     * <code>a + b</code>.
     *
     * @param a le premier opérande
     * @param b le second opérande
     * @return a + b si tout se passe bien
     */
    public static int additionner(int a, int b) {
	if (a < 0 || b < 0) {
	    throw new IllegalArgumentException("Les arguments doivent être des nombres positifs.");
	}
	Map<Entree, Boolean> env = new HashMap<Entree, Boolean>();

	int nbBits = (int) Math.ceil(Math.max(Math.log(a) / Math.log(2),
					      Math.log(b) / Math.log(2))) + 2;	
	nbBits = nbBits <= 0 ? 1 : nbBits;
	Element[] sorties = Additionneur.additionneur(nbBits);
	
	int k = 0;
	while (k < nbBits) {
	    env.put(new Entree("a" + k), a % 2 == 1 ? true: false);
	    a /= 2;
	    env.put(new Entree("b" + k), b % 2 == 1 ? true: false);
	    b /= 2;
	    k++;
	}
	env.put(new Entree("retenue d'entrée"), false);

	int resultat = 0;
	for (int i = nbBits - 1; i >= 0; i--) {
	    resultat <<= 1;
	    resultat += sorties[i].evaluer(env) ? 1 : 0;
	}
	return resultat;
    }

    /**
     * Cette méthode renvoie un tableau d'éléments constituant
     * l'ensemble des sorties d'un additionneur booléen.
     *
     * @param nbBits la taille, en nombre de bits, des entiers traités par l'additionneur
     * @return un tableau de sorties représentant le circuit additionneur.
     */
    public static Element[] additionneur(int nbBits) {
	Entree[] a = new Entree[nbBits];
	Entree[] b = new Entree[nbBits];
	Element[] resultat = new Element[nbBits];
	Element[] sorties = new Element[nbBits + 1];
	Element[] retenues = new Element[nbBits + 1];
	retenues[0] = new Entree("retenue d'entrée");

	for(int k = 0; k < nbBits; k++) {
	    a[k] = new Entree("a" + k);
	    b[k] = new Entree("b" + k);
	    Element[] sortieAdditionneur = Additionneur.additionneurElementaire(a[k], b[k], retenues[k], k);
	    resultat[k] = sortieAdditionneur[0];
	    retenues[k + 1] = sortieAdditionneur[1];
	}
	for (int k = 0; k < nbBits; k++) {
	    sorties[k] = resultat[k];
	}
	sorties[nbBits] = retenues[nbBits];
	return sorties;
    }

    private static Element xor(String name, Element e1, Element e2) {
	return new PorteOu(name, new PorteEt(anonymousName(), e1, new Inverseur(anonymousName(), e2 )),
			   new PorteEt(anonymousName(), new Inverseur(anonymousName(), e1), e2));
    }

    private static Element[] additionneurElementaire(Element a, Element b, Element cIn, int k) {
	Element porte1 = Additionneur.xor(anonymousName(), a, b);
	Element sortie = Additionneur.xor("sortie " + k, porte1, cIn);
	Element cOut = new PorteOu("retenue " + k, new PorteEt(anonymousName(), porte1, cIn),
				   new PorteEt(anonymousName(), a, b));
	Element[] tab = {sortie, cOut};
	return tab;
    }

    private static String anonymousName() {
	Additionneur.nbElements++;
	return "anonyme " + Additionneur.nbElements;
    }
}
