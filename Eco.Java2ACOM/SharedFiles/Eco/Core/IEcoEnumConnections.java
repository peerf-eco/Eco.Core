package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoEnumConnections extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46});

    short Next(int cConnections, EcoConnectionData rgcd, Pointer<Integer> cFetched);
    short Skip(int cConnections);
    short Reset();
    short Clone(Pointer<IEcoUnknown> pCP);
}
