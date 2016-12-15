package im.vinci.testfaac;

import android.app.ProgressDialog;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import audio.faac.EncodedData;
import audio.faac.FaacCodec;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    private ProgressDialog mProgressDialog;
    private VTrackProcessor mTrackProcessor;
    private FaacCodec mCodec;
    private int BUFFER_SIZE;

    private FileOutputStream mFos;
    private String mEncodePath = "/mnt/sdcard/mix/encode.aac";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        ButterKnife.bind(this);

        findViewById(R.id.btnEncode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onClickEncode();
            }
        });

//        BUFFER_SIZE = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);

        mCodec = new FaacCodec();
        BUFFER_SIZE = mCodec.init(44100, 2, 16);
    }

//    @OnClick({R.id.btnEncode})
    void onClickEncode(){
        Logger.i(TAG, "onClickEncode");
        showLoadingDlg("Encoding...");

        if (mTrackProcessor == null){
            mTrackProcessor = new VTrackProcessor();
        }
        mTrackProcessor.start();

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {

                byte[] buffer = new byte[BUFFER_SIZE];
                while (mTrackProcessor.isProcessing()){
                    int read = mTrackProcessor.getData(buffer, BUFFER_SIZE);
                    if (read <= 0){
                        continue;
                    }
//                    byte[] littleBuffer = new byte[read];
//                    ByteBuffer.wrap(buffer, 0, read).order(ByteOrder.BIG_ENDIAN).get(littleBuffer);
//                    EncodedData data = mCodec.encode(littleBuffer, read);
//                    int inputLen = read * 2;
//                    byte[] inputBuf = new byte[inputLen];
//                    int rp = 0;
//                    boolean isNeg = false;
//                    for (int i = 0; i < inputLen; ++i){
//                        if ((i - 1)%4 == 0 ){//4n+1的位置写入
//                            if (rp < read){
//                                inputBuf[i] = buffer[rp++];
//                            }
//                        }
//                        if ((i - 2)%4 == 0){//4n+2的位置写入
//                            if (rp < read){
//                                inputBuf[i] = buffer[rp];
//                                if (buffer[rp] < 0){
//                                    isNeg = true;
//                                }
//                                rp++;
//                            }
//                        }
//                        if ((i - 3)%4 == 0){
//                            if (isNeg){
//                                inputBuf[i] = -1;
//                            }
//                            isNeg = false;
//                        }
//                    }
//                    EncodedData data = mCodec.encode(inputBuf, inputLen);

                    EncodedData data = mCodec.encode(buffer, read);
                    writeToFile(data);
                }

                if (mFos != null){
                    try {
                        mFos.flush();
                        mFos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        hideLoadingDlg();
                    }
                });

                return null;
            }
        }.execute();
    }

    @Override
    protected void onDestroy() {
//        ButterKnife.unbind(this);
        mCodec.uninit();
        super.onDestroy();
    }

    private void showLoadingDlg(String content){
        hideLoadingDlg();
        mProgressDialog = ProgressDialog.show(this, null, content, true, true);
    }

    private void hideLoadingDlg(){
        if (mProgressDialog != null){
            mProgressDialog.dismiss();
            mProgressDialog = null;
        }
    }

    private void writeToFile(EncodedData data){
        if (data == null){
            Logger.e(TAG, "encode data is null");
            return;
        }

        if (data.getBufferSize() <= 0){
            Logger.e(TAG, "encode data size is < 0");
            return;
        }

        if (mFos == null){
            File file = new File(mEncodePath);
            if (file.exists()) {
                file.delete();
            }
            try {
                mFos = new FileOutputStream(file);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                Logger.e(TAG, "open file failed[%s]", e.getMessage());
                return;
            }
        }

        try {
            Logger.i(TAG, "write to aac file begin[%d]", data.getBufferSize());
//            byte[] wf = new byte[data.getBufferSize()];
//            ByteBuffer.wrap(data.getBuffer(), 0, data.getBufferSize()).order(ByteOrder.LITTLE_ENDIAN).get(wf);
            mFos.write(data.getBuffer(), 0, data.getBufferSize());
//            mFos.write(wf);
            Logger.i(TAG, "write to aac file end[%d]", data.getBufferSize());
        } catch (IOException e) {
            e.printStackTrace();
            Logger.e(TAG, "write aac failed[%s]", e.getMessage());
        }

    }
}
