package Eco.Mutex;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

@EcoNative
public interface IEcoMutex1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0xDD, 0x29, 0x2B, (byte) 0x8D, 0x75, 0x0B, 0x4A, 0x3C, (byte) 0xBB, (byte) 0x8D, 0x31, 0x6D, 0x60, (byte) 0x9A, 0x39, (byte) 0x8A});

    void Lock();
    void UnLock();
}
