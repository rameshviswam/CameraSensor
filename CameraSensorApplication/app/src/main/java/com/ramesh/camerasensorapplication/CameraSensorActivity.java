package com.ramesh.camerasensorapplication;


import android.app.Activity;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
        import android.view.Menu;
        import android.view.MenuItem;
        import android.widget.TextView;


public class CameraSensorActivity extends Activity {
    private TCPServerThread testServer;
    private Thread mServerThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera_sensor);

        testServer = new TCPServerThread();
        mServerThread = new Thread(testServer);
        mServerThread.start();

        TextView text = (TextView) findViewById(R.id.test_info);
    }
}

