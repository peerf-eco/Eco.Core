package Eco.Core;

public class UGUID {
    byte Preamble;
    byte Length;
    byte[] Data;

    public UGUID(byte Preamble, byte Length, byte... Data) {
        this.Preamble = Preamble;
        this.Length = Length;
        this.Data = Data;
    }
}
