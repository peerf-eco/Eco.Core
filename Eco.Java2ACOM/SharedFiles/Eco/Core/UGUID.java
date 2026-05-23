package Eco.Core;

import java.util.Arrays;

public class UGUID {
    public byte Preamble;
    public byte Length;
    public byte[] Data;

    public UGUID() {
        Preamble = 0;
        Length = 0;
        Data = new byte[]{};
    }

    public UGUID(byte Preamble, byte Length, byte... Data) {
        this.Preamble = Preamble;
        this.Length = Length;
        this.Data = Data;
    }

    public boolean equals(UGUID other) {
        return Preamble == other.Preamble && Length == other.Length && Arrays.equals(Data, other.Data);
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        for (byte i : Data) {
            byte b = (byte) ((i & 0xFF) >> 4);
            if (b < 10) {
                result.append((char) (b + '0'));
            } else {
                result.append((char) ((b - 10) + 'A'));
            }
            b = (byte) (i & 0xF);
            if (b < 10) {
                result.append((char) (b + '0'));
            } else {
                result.append((char) ((b - 10) + 'A'));
            }
        }
        return result.toString();
    }
}
