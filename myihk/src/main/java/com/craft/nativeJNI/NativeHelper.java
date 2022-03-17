package com.craft.nativeJNI;

import com.alibaba.fastjson.JSONObject;

public class NativeHelper {

    public native static void startNewPhone(JSONObject obj);
    public native static void startMethodTrace();
}
