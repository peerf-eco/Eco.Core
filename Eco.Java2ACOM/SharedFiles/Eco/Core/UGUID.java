package Eco.Core;

import java.util.Arrays;

public class UGUID {
    byte Preamble;
    byte Length;
    byte[] Data;

    public UGUID() {}

    public UGUID(byte Preamble, byte Length, byte... Data) {
        this.Preamble = Preamble;
        this.Length = Length;
        this.Data = Data;
    }

    public boolean equals(UGUID other) {
        return Preamble == other.Preamble && Length == other.Length && Arrays.equals(Data, other.Data);
    }
}
