package com.android.server;

import android.os.IHelloService;

public class HelloService extends IHelloService.Stub {

    private int ptr = 0;
    HelloService(){
        ptr = init_hello();
    }

    public int getVal(){
        return getVal_native(ptr);
    }

    public void setVal(int val){
        setVal_native(ptr,val);
    }

    public native int init_hello();
    public native int getVal_native(int ptr);
    public native void setVal_native(int ptr,int val);

}

