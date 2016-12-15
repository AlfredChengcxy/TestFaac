package im.vinci.testfaac;

import android.text.TextUtils;
import android.util.Log;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Created by zhonglz on 2016/11/25.
 */
public class VTrackProcessor {
    private final static String TAG = VTrackProcessor.class.getSimpleName();
    private final static int TRACK_BUFFER_SIZE = 10 * 1024;

    private String mPath = "/mnt/sdcard/mix/sin.wav";
    private FileInputStream mFileInputStream;
    private boolean isProcessing = false;


    public VTrackProcessor(){

    }

    public void start(){
        if (TextUtils.isEmpty(mPath)){
            Log.e(TAG, "path is empty....");
            return;
        }

        try {
            isProcessing = true;
            mFileInputStream = new FileInputStream(mPath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Logger.e(TAG, "file input stream failed [%s]", e.getMessage());
        }

    }

    public void stop(){
        isProcessing = false;

        if (mFileInputStream != null) {
            try {
                mFileInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            mFileInputStream = null;
        }
    }

    public void setTrackPath(String path){
        mPath = path;
    }

    public boolean isProcessing(){
        return isProcessing;
    }

    public int getData(byte[] outputData, int size){

        if (!isProcessing){
            Log.e(TAG, "not processing....");
            return 0;
        }

        if (mFileInputStream == null){
            Log.e(TAG, "file input stream null....");
            return 0;
        }

        int ret;
        try {
            ret = mFileInputStream.read(outputData, 0, size);

            if (ret == -1){
                Log.e(TAG, "track file end....");
                stop();
            }
        } catch (IOException e) {
            e.printStackTrace();
            return 0;
        }
        Logger.i(TAG, "getData size[%d], ret[%d]", size, ret);
        return ret;
    }


}
