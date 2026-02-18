package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;

public class IEcoFile_Client extends IEcoUnknown_Client implements IEcoFile {
    @Override
    public long get_Size() {
        return (Long) invokeNative(3, new Object[]{}, long.class);
    }

    @Override
    public String get_Name() {
        return (String) invokeNative(4, new Object[]{}, String.class);
    }

    @Override
    public IEcoFileInfo get_Info() {
        return (IEcoFileInfo) invokeNative(5, new Object[]{}, IEcoFileInfo.class);
    }

    @Override
    public long get_Pointer() {
        return (Long) invokeNative(6, new Object[]{}, long.class);
    }

    @Override
    public void set_Pointer(long value) {
        invokeNative(7, new Object[]{}, void.class);
    }

    @Override
    public int Read(long pv, Long pSize) {
        return (Integer) invokeNative(8, new Object[]{pv, pSize}, int.class);
    }

    @Override
    public int Write(long pv, Long pSize) {
        return (Integer) invokeNative(9, new Object[]{pv, pSize}, int.class);
    }

    @Override
    public int Close() {
        return (Integer) invokeNative(10, new Object[]{}, int.class);
    }

    @Override
    public long get_Descriptor() {
        return (Long) invokeNative(11, new Object[]{}, long.class);
    }
}
