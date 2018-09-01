package com.omegastar.carem;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.BufferedInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {

    private BroadcastReceiver receiver;
    private TextView comResult;
    private ProgressBar pbCom;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        comResult = findViewById(R.id.comResult);
        comResult.setBackgroundColor(getResources().getColor(R.color.comNoaction));
        pbCom = findViewById(R.id.pbCom);
        pbCom.setVisibility(View.INVISIBLE);

        // Prepare for the answer
        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String s = intent.getStringExtra(carEmBackgroundTask.CAREM_MESSAGE);
                Log.d("CAREM-MAIN result:", s);
                if (s.equals("OK")){
                    comResult.setBackgroundColor(getResources().getColor(R.color.comOK));
                    pbCom.setVisibility(View.INVISIBLE);
                }
                else {
                    comResult.setBackgroundColor(getResources().getColor(R.color.comKO));
                    pbCom.setVisibility(View.INVISIBLE);
                }
                /*TextView textView = findViewById(R.id.text2display);
                textView.setText("Car at "+carip+" is "+s);*/

            }
        };
    }

    private String getCarIp () {
        EditText editText = (EditText) findViewById(R.id.etIp);
        String carip = editText.getText().toString();
        return carip;
    }

    private void intent2car (String rest,String tag,String value) {
        Intent intent = new Intent(this, carEmBackgroundTask.class);
        intent.putExtra("rest",rest);
        intent.putExtra("tag",tag);
        intent.putExtra("value",value);

        EditText editText = (EditText) findViewById(R.id.etIp);
        String carip = editText.getText().toString();
        intent.putExtra("CARIP",getCarIp());
        comResult.setBackgroundColor(getResources().getColor(R.color.comOnprogress));
        pbCom.setVisibility(View.VISIBLE);
        startService(intent);
    }

    public void forward(View view)      {intent2car("mode","mode","F");}
    public void backwards(View view)    {intent2car("mode","mode","B");}
    public void stop(View view)         {intent2car("mode","mode","S");}
    public void left(View view)         {intent2car("steeringWheel","raw","L");}
    public void right(View view)        {intent2car("steeringWheel","raw","R");}
    public void gas(View view)          {intent2car("speed","raw","G");}
    public void brake(View view)        {intent2car("speed","raw","B");}
    public void turnLeft(View view)     {intent2car("mode","mode","L");}
    public void turnRight(View view)    {intent2car("mode","mode","R");}
    public void uturn(View view)        {intent2car("steeringWheel","raw","U");}


    public void apply(View view) {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        intent.putExtra("CARIP",getCarIp());
        startActivity(intent);
        comResult.setBackgroundColor(getResources().getColor(R.color.comNoaction));
        pbCom.setVisibility(View.INVISIBLE);
    }


    @Override
    protected void onStart() {
        super.onStart();
        LocalBroadcastManager.getInstance(this).registerReceiver((receiver),
                new IntentFilter(carEmBackgroundTask.CAREM_RESULT)
        );
    }

    @Override
    protected void onStop() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(receiver);
        super.onStop();
    }
}


