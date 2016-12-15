package audio.faac;

/**
 * Created by zhonglz on 2016/12/2.
 */
public class FaacCodec {
    private final static String TAG = FaacCodec.class.getSimpleName();

    static {
        System.loadLibrary("vfaac");
    }

    public static native int               init(int sampleRate, int channels, int bits);
    public static native EncodedData       encode(byte[] inputbuffer, int size);
    public static native int               uninit();
    public static native void              setDebug(boolean isDebug);

}
