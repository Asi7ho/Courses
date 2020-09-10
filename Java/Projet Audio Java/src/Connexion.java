public class Connexion {
    private CommunicationPort upModulePort;
    private CommunicationPort downModulePort;

    public  Connexion(CommunicationPort upModulePort, CommunicationPort downModulePort) {
        this.upModulePort = upModulePort;
        this.downModulePort = downModulePort;
    }

    public void communicate() {
        downModulePort.setValue(upModulePort.getValue());
    }
}
