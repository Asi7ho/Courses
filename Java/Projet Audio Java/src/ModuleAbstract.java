import java.util.LinkedList;
import java.util.List;

public abstract class ModuleAbstract {
    static int SAMPLE_FREQ = 44100;
    public String nom;
    private CommunicationPort[] inputPort;
    private CommunicationPort[] outputPort;

    public ModuleAbstract(String nom, int nbInputPort, int nbOutputPort) {
        this.nom = nom;
        this.inputPort = new CommunicationPort[nbInputPort];
        this.outputPort = new CommunicationPort[nbOutputPort];
        for (int i = 0; i < nbInputPort; i++) {
            inputPort[i] = new CommunicationPort(this, i);
        }
        for (int i = 0; i < nbOutputPort; i++) {
            outputPort[i] = new CommunicationPort(this, i);
        }
    }

    @Override
    public int hashCode() {
        return this.nom.hashCode();
    }

    public String getName() {
        return nom;
    }

    public int getNbInputPorts() {
        return inputPort.length;
    }

    public int getNbOutputPorts(){
        return outputPort.length;
    }

    public CommunicationPort[] getInputPort() {
        return inputPort;
    }

    public CommunicationPort[] getOutputPort() {
        return outputPort;
    }

    public static Connexion connect(ModuleAbstract mOutput, int idOutputPort, ModuleAbstract mInput, int idInputPort) {
        Connexion conn = new Connexion(mOutput.outputPort[idOutputPort], mInput.inputPort[idInputPort]);
        mOutput.getOutputPort()[idOutputPort].setConnexion(conn);
        return conn;
    }

    public boolean isConnectedInputPort(int inputPortId){
        if (inputPort[inputPortId].isConnected()){
            return true;
        } else {
            return false;
        }
    }

    public boolean isConnectedOutputPort(int outputPortId){
        if (outputPort[outputPortId].isConnected()){
            return true;
        } else {
            return false;
        }
    }

    public void setAndSendOutputPortValue(int idOutputPort, double sampleValue) {
        outputPort[idOutputPort].setValue(sampleValue);
        //System.out.println(outputPort[idOutputPort].isConnected());
        if (outputPort[idOutputPort].isConnected()){
            outputPort[idOutputPort].getConnexion().communicate();
        }

    }

    public List<CommunicationPort> getUnconnectedInputPorts() {
        LinkedList<CommunicationPort> unconnected = new LinkedList<CommunicationPort>();
        for (int i = 0; i < inputPort.length; i++) {
            if (!inputPort[i].isConnected()){
                unconnected.add(inputPort[i]);
            }
        }
        return unconnected;
    }

    public List<CommunicationPort> getUnconnectedOutputPorts() {
        LinkedList<CommunicationPort> unconnected = new LinkedList<CommunicationPort>();
        for (int i = 0; i < outputPort.length; i++) {
            if (!outputPort[i].isConnected()){
                unconnected.add(outputPort[i]);
            }
        }
        return unconnected;
    }

    double getInputPortValue(int idInputPort) {
        //System.out.println(inputPort[idInputPort].getValue());
        return inputPort[idInputPort].getValue();
    }

    public void setInputPortValue(int idInputPort, double sampleValue) {
        inputPort[idInputPort].setValue(sampleValue);
    }

    public abstract void exec();

    public abstract void reset();

}
