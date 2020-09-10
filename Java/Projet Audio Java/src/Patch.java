import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class Patch {
    private String nom;
    private LinkedList<ModuleAbstract> listeModules;
    private LinkedList<Connexion> listeConnexions;
    private Map<String, ModuleAbstract> collecModule;

    public Patch(String nom) {
        this.nom = nom;
        this.listeModules = new LinkedList<ModuleAbstract>();
        this.listeConnexions = new LinkedList<Connexion>();
        this.collecModule = new HashMap<String, ModuleAbstract>();
    }

    public void addModule(ModuleAbstract m) {
        listeModules.add(m);
        collecModule.put(m.getName(), m);
    }

    public List<CommunicationPort> getUnconnectedInputPorts() {
        LinkedList<CommunicationPort> unconnected = new LinkedList<CommunicationPort>();
        for (ModuleAbstract m : listeModules) {
            unconnected.addAll(m.getUnconnectedInputPorts());
        }
        return unconnected;
    }

    public List<CommunicationPort> getUnconnectedOutputPorts() {
        LinkedList<CommunicationPort> unconnected = new LinkedList<CommunicationPort>();
        for (ModuleAbstract m : listeModules) {
            unconnected.addAll(m.getUnconnectedOutputPorts());
        }
        return unconnected;
    }

    public void connect(String nameOfOutputModule, int idOutputPort, String nameOfInputModule, int idInputPort) {
        Connexion conn = ModuleAbstract.connect(collecModule.get(nameOfOutputModule), idOutputPort, collecModule.get(nameOfInputModule), idInputPort);
        listeConnexions.add(conn);
    }

    public void exec() {
        for (ModuleAbstract m : listeModules) {
            m.exec();
        }
    }

    public void exec(int nbStep) {
        for (int i = 0; i < nbStep; i++){
            for (ModuleAbstract m : listeModules) {
                m.exec();
            }
        }
    }

    void reset() {
        for (ModuleAbstract m : listeModules) {
            m.reset();
        }
    }

}
