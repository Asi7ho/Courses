package zoo;
import java.util.HashSet;
import java.util.LinkedList;

import zoo.animal.*;

/** Une classe représentant un Zoo : 
 * 
 * Un zoo a un nom, et peut contenir un nombre quelconque d'animaux.
 * On peut : ajouter un nouvel animal, récupérer le nombre d'animaux, calculer le cout de nourriture
 * total, récupérer une chaine de caractères représentant l'état du Zoo.
 *
 */public class Zoo {
	private String nom;
	private LinkedList<Animal> animaux;
	private HashSet<Animal> animauxhs;
	
	/** Construit le zoo de nom name, initialement vide */
	public Zoo(String name) {
		nom=name;
		animaux = new LinkedList<Animal>();
		animauxhs = new HashSet<Animal>();
	}

	/** Ajoute l'animal a au zoo */
	public void ajoute(Animal a) {
		if (animauxhs.add(a)) {
			animaux.add(a);
		} else {
			return;
		}
	}
	
	/** Retourne le nombre d'animaux contenu dans le zoo */
	public int nbAnimaux() { 
		return animaux.size();
	}
	
	/** Retourne le cout total du zoo par jour (cout de nourriture) */
	public double coutTotal() {
		double cout = 0;
		for(Animal a : animaux) {
			cout += a.getCout();
		}
		return cout;
	}

	public Animal recupereAnimal(String nom) {
		for (Animal a : animaux) {
			if (a.equals(nom)) {
				return a;
			}
		}
		return null;
	}

	public void supprime(String nom) {
		animaux.remove(recupereAnimal(nom));
	}
	
	/** Retourne une chaine de caractères repréentant l'état du zoo et de tous ces animaux */
	public String toString() { 
		String s= "Zoo:"+nom+" avec "+nbAnimaux()+" animaux\n";
		for (Animal a : animaux) s += "     " + a + "\n";
		return s;
	}
}
 