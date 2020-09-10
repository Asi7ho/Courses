public abstract class PorteBinaire extends Element{
    protected Element source1;
    protected Element source2;

    public PorteBinaire(String name, Element source1, Element source2) {
        super(name);
        if (source1 == null || source2 == null) {
            throw new IllegalArgumentException("Les sources ne doivent pas etre nulles");
        }
        this.source1 = source1;
        this.source2 = source2;
    }
}
