public class GenSineBasic extends ModuleAbstract {

    private double freq;
    private double amp;
    private int pas;

    public GenSineBasic(String nom, double freq, double amp) {
        super(nom, 0, 1);
        this.freq = freq;
        this.amp = amp;
        this.pas = 0;
    }

    private void avancePas() {
        pas += 1;
    }

    @Override
    public void exec() {
        int i = 0;
        double e = amp*Math.sin(2*Math.PI*freq*pas/SAMPLE_FREQ);
        setAndSendOutputPortValue(0, e);
        avancePas();
        //System.out.println(e);
    }

    @Override
    public void reset() {
        pas = 0;
    }
}
