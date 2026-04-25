package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoEnumConnectionPoints extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46});

    int Next(int cConnections, IEcoUnknownPtr pCP, IntegerPtr cFetched);
    int Skip(int cConnections);
    int Reset();
    int Clone(IEcoUnknownPtr pCP);
}
