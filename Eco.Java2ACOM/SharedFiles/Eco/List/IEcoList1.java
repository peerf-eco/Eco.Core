package Eco.List;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

@EcoNative
public interface IEcoList1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x5A, (byte) 0xAD, (byte) 0xBC, (byte) 0xB4, (byte) 0x84, 0x6C, 0x45, 0x76, (byte) 0x82, 0x7B, 0x28, 0x7B, 0x5E, 0x67, (byte) 0xA1, 0x52});

    int Count();
    Object Item(int index);
    int Add(Object value);
    int IndexOf(Object value);
    void InsertAt(int index, Object value);
    void Remove(Object value);
    void RemoveAt(int index);
    void Clear();
}
