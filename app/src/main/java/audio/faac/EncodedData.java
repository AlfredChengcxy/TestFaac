package audio.faac;

/**
 * Created by zhonglz on 2016/12/2.
 */
public class EncodedData {
    private final static String TAG = EncodedData.class.getSimpleName();

    private byte[] buffer;
    private int bufferSize;
    private int timestamp;
    private boolean isKeyFrame;//是否关键

    public EncodedData(byte[] buffer, int bufferSize, int timestamp, boolean isKeyFrame) {
        this.buffer = buffer;
        this.bufferSize = bufferSize;
        this.timestamp = timestamp;
        this.isKeyFrame = isKeyFrame;
    }

    public byte[] getBuffer() {
        return buffer;
    }

    public void setBuffer(byte[] buffer) {
        this.buffer = buffer;
    }

    public int getBufferSize() {
        return bufferSize;
    }

    public void setBufferSize(int bufferSize) {
        this.bufferSize = bufferSize;
    }

    public int getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(int timestamp) {
        this.timestamp = timestamp;
    }

    public boolean isKeyFrame() {
        return isKeyFrame;
    }

    public void setKeyFrame(boolean keyFrame) {
        isKeyFrame = keyFrame;
    }
}
