/** Une classe représentant la notion de nombre complexe, 
 * dotée des principales opération arithmétiques complexes.
 * @author desvignes, castagne
 *
 */
public class Complexe {
	private double re,im;

	/** Construit le complexe nul */
	public Complexe() {
	}

	/** Construit le complexe (re+i*im) */
	public Complexe(double re, double im) { 
		this.re=re; 
		this.im=im;
	}

	/** Constructeur de copie
	 */
	public Complexe(Complexe c) { 
		// On peut ecrire Complexe(c.re,c.im);
		// On peut ecrire Complexe(c.getReelle(),c.getImaginaire());
		re=c.re; 
		im=c.im;
	}

	/** @return la partie réelle de this*/
	public double getReelle(){
		return re;
	}

	/** @return la partie imaginaire de this*/
	public double getImaginaire() {
		return im; 
	}

	/** @return le module de this*/
	public double getRho(){ 
		return Math.sqrt(re*re+im*im);
	}

	/** @return l'argument de this*/
	public double getTheta(){ 
		return Math.atan2(im,re); 
	}

	/** Affecte la partie réelle de this à a, sans modifier sa partie imaginaire*/
	public void setReelle(double a) { 
		re=a; 
	}

	/** Affecte la partie imaginaire de this à a, sans modifier sa partie réelle*/
	public void setImaginaire(double a) { 
		im=a; 
	}

	/** Affecte le module de this à mod, sans modifier son argument*/
	public void setRho(double mod) { 
		// Attention : on va changer re, 
		// donc getTheta deviendrait faux après l'affectation de re pour le calcul de im
		// => on stocke d'abord theta dans une variable locale
		double arg=getTheta();
		re=mod*Math.cos(arg); 
		im=mod*Math.sin(arg); 
	}

	/** Affecte l'agument de this à arg, sans modifier son module*/
	public void setTheta(double arg) { 
		// Attention : on va changer re, donc getRho deviendrait faux
		double r=getRho();
		re=r*Math.cos(arg); 
		im=r*Math.sin(arg); 
	}

	/** Affectation globale : this := newValue*/
	public void set(Complexe newValue){
		setReelle(newValue.getReelle());
		setImaginaire(newValue.getImaginaire());
	}

	/** Addition en place.
	 *  this := this + c
	 */
	public void add(Complexe c) {
		setReelle(getReelle()+c.getReelle());
		setImaginaire(getImaginaire()+c.getImaginaire());
	}

	/** Addition en place.
	 *  this := this + k
	 */
	public void add(double k) {
		setReelle(getReelle()+k);
	}

        /** Addition de deux complexes dans un nouveau complexe.
	 *  @return un nouveau complexe égal à a+b
	 */
	public static Complexe add(Complexe a, Complexe b) {		
		return new Complexe(a.getReelle()+b.getReelle(), a.getImaginaire()+b.getImaginaire());
	}

	/** Soustraction en place.
	 *  this := this - c
	 */
	public void sub(Complexe c) {
		setReelle(getReelle()-c.getReelle());
		setImaginaire(getImaginaire()-c.getImaginaire());
	}

	/** Soustraction en place.
	 *  this := this - k
	 */
	public void sub(double k) {
		setReelle(getReelle()-k);
	}


	/** Soustraction de deux complexes.
	 *  @return un nouveau complexe égal à a-b
	 */
	public static Complexe sub(Complexe a, Complexe b) {
		return new Complexe(a.getReelle()-b.getReelle(), a.getImaginaire()-b.getImaginaire());
	}

	/** Multiplication en place.
	 *  this := this * c
	 */	public void mult(Complexe c)  {
		setRho(getRho() * c.getRho());
		setTheta(getTheta() + c.getTheta());
	}

	/** Multiplication en place.
	 *  this := this * k
	 */
	public void mult(double k) {
		setRho(getRho()*k);
		setTheta(getTheta());
	}
	
	/** Multiplication de deux complexes.
	 *  @return un nouveau complexe égal à a*b
	 */
	public static Complexe mult(Complexe a, Complexe b) {
		double rho = a.getRho() * b.getRho();
		double theta = a.getTheta() + b.getTheta();
		Complexe res = new Complexe(1, 0);
		res.setRho(rho);
		res.setTheta(theta);
		return res;
	}


	/** Division en place.
	 *  this := this / b
	 *  @throws ArithmeticException si c est le complexe nul
	 */
	public void divide(Complexe c)  {
		setRho(getRho()/c.getRho());
		setTheta(getTheta()-c.getTheta());
	}

	/** Division en place.
	 *  this := this / k
	 *  @throws ArithmeticException si k est nul
	 */
	public void divide(double k)  {
		setRho(getRho()/k);
	}

	/** Division de deux complexes.
	 *  @return un nouveau complexe égal à a/b
	 *  @throws ArithmeticException si b est le complexe nul
	 */
	public static Complexe divide(Complexe a, Complexe b) {
		double rho = a.getRho() / b.getRho();
		double theta = a.getTheta() - b.getTheta();
		Complexe res = new Complexe(1, 0);
		res.setRho(rho);
		res.setTheta(theta);
		return res;
	}

	/** Complexe conjugué.
	 * @return le complexe conjugué de this
	 */
	public Complexe conjugate() {
		return new Complexe( getReelle(), - getImaginaire());
	}

	/** Complexe inverse 
	 *  @return le complexe inverse de this
	 *  @throws ArithmeticException si this est le complexe nul
	 */
	public Complexe invert() {
		Complexe res = new Complexe(1, 0);
		res.divide(this);
		return res;
	}


	/** Elevation à la puissance.
	 *  Transforme this a this^n
	 */
	public void pow(int n){
		Complexe tmp = new Complexe(this);
		this.setReelle(1);
		this.setImaginaire(0);
		for(int i = 0 ; i < n ; i ++){
			mult(tmp);
		}
	}

	/** Elevation à la puissance.
	 *  @return un nouveau complexe égal à c^n
	 */
	public static Complexe pow(Complexe c, int n){
		Complexe tmp = new Complexe(1, 0);
		for(int i = 0 ; i < n ; i ++){
			tmp.mult(c);
		}
		return tmp;
	}
	
	@Override
	public String toString() { 
		return getReelle() + " i * " + getImaginaire()  
		+ " [<=>  " 
		+ getRho() + "* exp(i*" + getTheta() + ") ]"; 
	}

	/** test d'égalité
	 * L'égalité testée est "au bit prêt".
	 */
	@Override
	public boolean equals(Object other){
		if(! (other instanceof Complexe) ) {
			return false;
		}
		Complexe c = ( Complexe ) other;
		return c.im == im && c.re == re;
	}


//	public static void main(String[] args) {
//		Complexe c1 = new Complexe(1, 0);
//		System.out.println("c1 => " + c1);
//
//		Complexe c2 = new Complexe(1, 1);
//		System.out.println("c2 => " + c2);
//
//		c1.add(c2);
//		System.out.println("c1 => " + c1);
//
//		c2.mult(c2);
//		System.out.println("c2 => " + c2);
//	}
}

