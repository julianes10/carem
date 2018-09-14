package com.omegastar.carem;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;

public class DisplayMessageActivity extends AppCompatActivity {

    private String carip;
    private BroadcastReceiver receiver;
    private ProgressBar pbCom;
    public boolean dd; //debug detail
    public boolean d;  //debug

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_message);

        dd=false;
        d=true;

        // Get the Intent that started this activity and extract the string
        Intent intent = getIntent();
        Bundle b = intent.getExtras();
        carip = b.getString("CARIP");

        TextView textView = findViewById(R.id.text2display);
        textView.setText("Checking if car at "+carip+" is available...");


        // Prepare for the answer
        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String s = intent.getStringExtra(carEmBackgroundTask.CAREM_MESSAGE);
                TextView textView = findViewById(R.id.text2display);
                textView.setText("Car at "+carip+" is "+s);
                pbCom.setVisibility(View.INVISIBLE);

            }
        };

        // Send the query
        Intent intent2 = new Intent(this, carEmBackgroundTask.class);
        intent2.putExtra("rest","status");
        intent2.putExtra("CARIP",carip);
        pbCom = findViewById(R.id.pbStatus);
        pbCom.setVisibility(View.VISIBLE);
        startService(intent2);
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
