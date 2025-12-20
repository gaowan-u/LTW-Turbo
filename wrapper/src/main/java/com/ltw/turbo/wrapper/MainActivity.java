package com.ltw.turbo.wrapper;


import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import java.io.IOException;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        PackageManager packageManager = getPackageManager();
        ApplicationInfo myAppInfo = getApplicationInfo();
        ApplicationInfo targetAppInfo = getAppicationInfo(packageManager);
        if(targetAppInfo == null) {
            finish();
            return;
        }
        String copySrc = myAppInfo.nativeLibraryDir;
        String copyDst = targetAppInfo.nativeLibraryDir;
        if(copySrc == null || copyDst == null || copySrc.isEmpty() || copyDst.isEmpty()) {
            Log.e("LTW-Turbo", "Wrong source or destination!");
            Log.e("LTW-Turbo", "Source: "+copySrc);
            Log.e("LTW-Turbo", "Destination: "+copyDst);
            finish();
            return;
        }
        String copyCommand = "cp " + copySrc + "/* "+copyDst+"/";
        try {
            Runtime.getRuntime().exec("am force-stop "+targetAppInfo.packageName);
            ProcessBuilder processBuilder = new ProcessBuilder("su", "-c", copyCommand);
            Process process = processBuilder.start();
            int errorCode = process.waitFor();
            if(errorCode == 0) {
                startActivity(packageManager.getLaunchIntentForPackage(targetAppInfo.packageName));
            }else {
                Log.e("LTW-Turbo", "Copying failed with error code "+errorCode);
            }
        }catch (Exception e) {
            Log.e("LTW-Turbo", "Failed to copy files!", e);
        }
        finish();
    }

    public ApplicationInfo getAppicationInfo(PackageManager packageManager) {
        try {
            return packageManager.getApplicationInfo("com.ltw.turbo", PackageManager.GET_SHARED_LIBRARY_FILES);
        }catch (PackageManager.NameNotFoundException e) {
            return null;
        }
    }
}