public class CommunicationPort {
    private int idPort;
    private ModuleAbstract m;
    private double sampleValue;
    private Connexion c;

    public CommunicationPort(ModuleAbstract m, int idPort){
        this.m = m;
        this.idPort = idPort;
    }

    public void setValue(double v) {
        sampleValue = v;
    }

    public double getValue() {
        return sampleValue;
    }

    public void setConnexion(Connexion c) {
        this.c = c;
    }

    public Connexion getConnexion() {
        return c;
    }

    public boolean isConnected() {
        if (c == null) {
            return false;
        } else {
            return true;
        }
    }


}
