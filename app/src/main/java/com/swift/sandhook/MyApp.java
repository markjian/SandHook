package com.swift.sandhook;

import android.app.Activity;
import android.app.Application;
import android.util.Log;

import com.swift.sandhook.testHookers.ActivityHooker;
import com.swift.sandhook.testHookers.CtrHook;
import com.swift.sandhook.testHookers.CustmizeHooker;
import com.swift.sandhook.testHookers.JniHooker;
import com.swift.sandhook.testHookers.LogHooker;
import com.swift.sandhook.testHookers.NewAnnotationApiHooker;
import com.swift.sandhook.testHookers.ObjectHooker;
import com.swift.sandhook.wrapper.HookErrorException;
import com.swift.sandhook.xposedcompat.XposedCompat;

import dalvik.system.DexClassLoader;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;

public class MyApp extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        SandHook.disableVMInline();

        try {
            SandHook.addHookClass(JniHooker.class,
                    CtrHook.class,
                    LogHooker.class,
                    CustmizeHooker.class,
                    ActivityHooker.class,
                    ObjectHooker.class,
                    NewAnnotationApiHooker.class);
        } catch (HookErrorException e) {
            e.printStackTrace();
        }

        //setup for xposed
        XposedCompat.cacheDir = getCacheDir();
        XposedCompat.context = this;
        XposedCompat.classLoader = getClassLoader();
        XposedCompat.isFirstApplication= true;

        XposedHelpers.findAndHookMethod(Activity.class, "onResume", new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                super.beforeHookedMethod(param);
                Log.e("XposedCompat", "beforeHookedMethod: " + param.method.getName());
            }

            @Override
            protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                super.afterHookedMethod(param);
                Log.e("XposedCompat", "afterHookedMethod: " + param.method.getName());
            }
        });

        try {
            ClassLoader classLoader = getClassLoader();
            DexClassLoader dexClassLoader = new DexClassLoader("/sdcard/hookers-debug.apk",
                    getCacheDir().getAbsolutePath(), null, classLoader);
            Class absHookerClass = Class.forName("com.swift.sandhook.hookers.AbsHooker", true, dexClassLoader);
            Class pluginHookerClass = Class.forName("com.swift.sandhook.hookers.PluginHooker", true, dexClassLoader);
            SandHook.addHookClass(getClassLoader(), absHookerClass, pluginHookerClass);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (HookErrorException e) {
            e.printStackTrace();
        }

    }
}
