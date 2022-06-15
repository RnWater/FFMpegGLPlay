package com.henry.ffmpegglplay;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatSeekBar;
import androidx.core.app.ActivityCompat;
import android.Manifest;
import android.annotation.TargetApi;
import android.content.ContentUris;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements Runnable, AppCompatSeekBar.OnSeekBarChangeListener{
    static {
        System.loadLibrary("ffmpegglplay");
    }

    private TextView showVideoTime;
    private AppCompatSeekBar video_seek;
    private TextView showPlayTime;
    private MyOpenGLVideoView videoView;
    private ImageView openVideo;
    private Thread th;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //去掉标题栏
        supportRequestWindowFeature( Window.FEATURE_NO_TITLE);
        //全屏，隐藏状态
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN ,
                WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        //屏幕为横屏
        setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );
        boolean result = (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED);
        if (!result) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0x01);
            }
        } else {
            setContentView(R.layout.activity_main);
            showVideoTime = findViewById(R.id.showVideoTime);
            video_seek = findViewById(R.id.video_seek);
            video_seek.setOnSeekBarChangeListener(this);
            video_seek.setMax(1000);
            showPlayTime = findViewById(R.id.showPlayTime);
            videoView = findViewById(R.id.videoView);
            videoView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    setPlayState(!getPlayState());
                }
            });
            openVideo = findViewById(R.id.openVideo);
            openVideo.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Intent intent = new Intent();
                    intent.setType("video/*|image/*"); //选择视频 (mp4 3gp 是android支持的视频格式)
                    intent.setAction(Intent.ACTION_GET_CONTENT);
                    startActivityForResult(intent, 1);
                }
            });
            //启动播放进度线程
            th = new Thread(this);
            th.start();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        handleImageOnkitKat(data);//高于4.4版本使用此方法处理图片
    }
    @TargetApi(19)
    private void handleImageOnkitKat(Intent data) {
        String filePath = null;
        Uri uri = data.getData();
        if (DocumentsContract.isDocumentUri(this, uri)) {
            //如果是document类型的uri，则通过document id处理
            String docId = DocumentsContract.getDocumentId(uri);
            if ("com.android.providers.media.documents".equals(uri.getAuthority())) {
                String id = docId.split(":")[1];//解析出数字格式的id
                String selection = MediaStore.Images.Media._ID + "=" + id;
                filePath = getImagePath(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, selection);
            } else if ("com.android,providers.downloads.documents".equals(uri.getAuthority())) {
                Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.valueOf(docId));
                filePath = getImagePath(contentUri, null);
            }

        } else if ("content".equalsIgnoreCase(uri.getScheme())) {
            filePath = getImagePath(uri, null);
        }
        Log.e("我的文件", "--->"+filePath);
        openVideo(filePath);
    }
    //获得图片路径
    public String getImagePath(Uri uri, String selection) {
        String path = null;
        Cursor cursor = getContentResolver().query(uri, null, selection, null, null); //内容提供器
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                int index = cursor.getColumnIndex(MediaStore.Images.Media.DATA);
                if (index >= 0) {
                    path = cursor.getString(index); //获取路径
                }
            }
        }
        cursor.close();
        return path;
    }

    public native void openVideo(String path);
    public native double getPlayPos();
    public native void setPlayState(boolean isP);
    public native boolean getPlayState();

    public native void seek(double pos);
    @Override
    public void run() {
        for(;;)
        {
            video_seek.setProgress((int)(getPlayPos()*1000));
            try {
                Thread.sleep( 40 );
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        seek((double) seekBar.getProgress() / (double)seekBar.getMax());
    }
}