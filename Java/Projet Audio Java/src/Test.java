import phelmaaudio.AudioData;

public class Test {
    public static void main(String [] arg) {
        int temps = 5;
        GenSineBasic gen = new GenSineBasic("sin", 442, 1.0);
        AudioDataReceiver output = new AudioDataReceiver("output");

        /*
        Connexion conn = ModuleAbstract.connect(gen, 0, output, 0);


        for (int i = 0; i < temps*ModuleAbstract.SAMPLE_FREQ; i++) {
            gen.exec(i);
            output.exec();
        }
        */

        Patch p = new Patch("Patch sin");
        p.addModule(gen);
        p.addModule(output);
        p.connect("sin", 0, "output", 0);

        p.exec(temps*ModuleAbstract.SAMPLE_FREQ);


        output.displayAudioDataWaveform();
        output.playAudioData();

    }

}
