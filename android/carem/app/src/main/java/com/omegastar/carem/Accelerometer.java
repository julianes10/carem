package com.omegastar.carem;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.app.Activity;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import static android.content.res.Configuration.ORIENTATION_PORTRAIT;
import static java.lang.Math.abs;

public class Accelerometer extends Activity implements SensorEventListener  {


    private SensorManager mSensorManager;
    private Sensor mSensor;
    private Button singleBt;
    private boolean playOn;
    float px=0;
    float py=0;
    float pz=0;
    private String carip;
    private int eventCounterLastLeftSlight=0;
    private int eventCounterLastRightSlight=0;
    private int eventCounterThreshold=6;

    public boolean dd; //debug detail
    public boolean d;  //debug

    private enum t_mode {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        STOP
    }
    t_mode stMode;
    private enum t_steeringWheel {
        NONE,
        LEFT,
        RIGHT,
    }
    t_steeringWheel stSteeringWheel;
    private enum t_pedal {
        NONE,
        GAS,
        BRAKE,
    }
    t_pedal stPedal;

    private PowerManager.WakeLock wakeLock;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_accelerometer);

        dd=false;
        d=true;

        Intent intent = getIntent();
        Bundle b = intent.getExtras();
        carip = b.getString("CARIP");

        singleBt = findViewById(R.id.btStartStop);
        singleBt.setText("Play");
        singleBt.setTextColor(Color.WHITE);
        singleBt.setTextSize(30);
        intent2car("mode","mode","S");
        stMode=t_mode.STOP;
        playOn=false;
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if (mSensor==null) {
            if (d) Log.d("CAREM", "no sensor available");
        }

        PowerManager powerManager = (PowerManager)getApplicationContext().getSystemService(Context.POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "My Lock");

    }



    public void playPause(View view) {
        if (playOn) {
            singleBt.setText("Play");
            playOn=false;
            stMode=t_mode.STOP;
            intent2car("mode","mode","S");
            wakeLock.release();

        } else {
            singleBt.setText("Pause");
            playOn=true;


            wakeLock.acquire();

        }
    }


    @Override
    protected void onStart() {
        super.onStart();


        if (mSensor != null) {
            mSensorManager.registerListener(this, mSensor,
                    SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    public void gas(View view)          {intent2car("speed","raw","G");}
    public void brake(View view)        {intent2car("speed","raw","B");}
    @Override
    protected void onStop() {
        super.onStop();
        mSensorManager.unregisterListener(this);
    }

    float x;
    float y;
    float z;


    private boolean isStop()
    {
        return Math.abs(x) < 2;
    }

    private boolean isForward()
    {
        return (Math.abs(x) > 2 && !isBackward());
    }

    private boolean isBackward()
    {
        return ((z < -2) && !isStop());
    }

    private boolean isLeft() {
        return (y > 6.5 && x < 0) || (y < -6.5 && x > 0);
    }

    private boolean isRight() {
        return (y > 6.5 && x > 0) || (y < -6.5 && x < 0);
    }

    private boolean isSlightLeft() {
        if ((y > 2 && y < 6.5 && x < 0) || (y < -2 && y > -6.5 && x > 0)) {
            if (dd) Log.d("CAREM", "isSlightLeft:" + eventCounterLastLeftSlight);
            if ((eventCounterLastLeftSlight == 0)  ||  (eventCounterLastLeftSlight > eventCounterThreshold)){
                    eventCounterLastLeftSlight=1;
                    return true;
            }
            else eventCounterLastLeftSlight+=1;
        }
        else eventCounterLastLeftSlight=0;
        return false;
    }

    private boolean isSlightRight() {
        if ((y > 2 && y < 5 && x > 0) || (y < -2 && y > -5 && x < 0)) {
            if (dd) Log.d("CAREM", "isSlightRight:" + eventCounterLastRightSlight);
            if ( (eventCounterLastRightSlight == 0) ||(eventCounterLastRightSlight > eventCounterThreshold)) {
                eventCounterLastRightSlight=1;
                return true;
            }
            else eventCounterLastRightSlight+=1;
        }
        else eventCounterLastRightSlight=0;
        return false;
    }


    public void onSensorChanged(SensorEvent event){

        if (playOn) {

            x=event.values[0];
            y=event.values[1];
            z=event.values[2];
            if (dd) Log.d("CAREM", "New sensor event X:" + event.values[0] + "\tY:" + event.values[1] + "\tZ:" + event.values[2]);



            switch (stMode) {
                case STOP:
                    if (isForward()) {
                        stMode = t_mode.FORWARD;
                        intent2car("mode", "mode", "F");
                    } else if (isBackward()) {
                        stMode = t_mode.BACKWARD;
                        intent2car("mode", "mode", "B");
                    }
                    break;
                case FORWARD:
                    if (isStop()) {
                        stMode = t_mode.STOP;
                        intent2car("mode", "mode", "S");
                    }
                    else if (isBackward()) {
                        stMode = t_mode.BACKWARD;
                        intent2car("mode", "mode", "B");
                    }
                    else if (isRight()) {
                        stMode = t_mode.RIGHT;
                        intent2car("mode", "mode", "R");
                    }
                    else if (isLeft()) {
                        stMode = t_mode.LEFT;
                        intent2car("mode", "mode", "L");
                    }
                    else if (isSlightLeft()) {
                         intent2car("steeringWheel", "raw", "L");
                    }
                    else if (isSlightRight()) {
                        intent2car("steeringWheel", "raw", "R");
                    }
                    break;
                case BACKWARD:
                    if (isStop()) {
                        stMode = t_mode.STOP;
                        intent2car("mode", "mode", "S");
                    }
                    else if (isForward()) {
                        stMode = t_mode.FORWARD;
                        intent2car("mode", "mode", "F");
                    }
                    else if (isRight()) {
                        stMode = t_mode.RIGHT;
                        intent2car("mode", "mode", "R");
                    }
                    else if (isLeft()) {
                        stMode = t_mode.LEFT;
                        intent2car("mode", "mode", "L");
                    }
                    else if (isSlightLeft()) {
                        intent2car("steeringWheel", "raw", "L");
                    }
                    else if (isSlightRight()) {
                        intent2car("steeringWheel", "raw", "R");
                    }
                    break;
                case RIGHT:
                    if (isRight()) {
                       //keep on doing
                    }
                    else if (isStop()) {
                        stMode = t_mode.STOP;
                        intent2car("mode", "mode", "S");
                    }
                    else if (isForward()) {
                        stMode = t_mode.FORWARD;
                        intent2car("mode", "mode", "F");
                    }
                    if (isBackward()) {
                        stMode = t_mode.BACKWARD;
                        intent2car("mode", "mode", "B");
                    }
                    else if (isLeft()) {
                        stMode = t_mode.LEFT;
                        intent2car("mode", "mode", "L");
                    }
                    break;
                case LEFT:
                    if (isLeft()) {
                        //keep on doing
                    } else if (isStop()) {
                        stMode = t_mode.STOP;
                        intent2car("mode", "mode", "S");
                    }
                    else if (isForward()) {
                        stMode = t_mode.FORWARD;
                        intent2car("mode", "mode", "F");
                    }
                    if (isBackward()) {
                        stMode = t_mode.BACKWARD;
                        intent2car("mode", "mode", "B");
                    }
                    else if (isRight()) {
                        stMode = t_mode.RIGHT;
                        intent2car("mode", "mode", "R");
                    }
                    break;
            }

/*
            if (  (( x <  1 ) && (stMode != t_mode.STOP)) )
            {
                stMode=t_mode.STOP;
                intent2car("mode","mode","S");
            } else if (    (x > 2 && x < 10 && z > -5 && y < 7 && y > -7 ) && (stMode!=t_mode.FORWARD)) {
                stMode=t_mode.FORWARD;
                intent2car("mode","mode","F");
            } else if (    (z < -5 && y < 7 && y > -7 ) && (stMode!=t_mode.BACKWARD))
                stMode=t_mode.BACKWARD;
                intent2car("mode", "mode", "B");
            } else if ( ( y > 7 ) && (stMode!=t_mode.BACKWARD)(! (py > 7))) {
                intent2car("mode","mode","R");
            }
            else if ( ( y < -7 ) && (! (py < -7))) {
                intent2car("mode","mode","L");
            }
*/
/*
            //Check slight RIGHT
            if ( y > 2 && y < 7 ) {
                if  (!(py > 2)) {
                    intent2car("steeringWheel","raw","R");
                }
            }

            //Check slight LEFT
            if (y < -2 && y > -7) {
                if  (!(py < -2)) {
                    intent2car("steeringWheel","raw","L");
                }
            }
*/


/*
            //Check +vel
            if (x > 8) {
                if  (px < 8) {
                    intent2car("speed","raw","G");
                }
            }
            //Check -vel
            if (x > 4) {
                if  (px > 4) {
                    intent2car("speed","raw","B");
                }
            }*/

            px=x;
            py=y;
            pz=z;

        }

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    private void intent2car (String rest,String tag,String value) {
        if (dd) Log.d("CAREM", "Event from accelerometer:"+rest+" "+tag+" "+value);
        Intent intent = new Intent(this, carEmBackgroundTask.class);
        intent.putExtra("rest",rest);
        intent.putExtra("tag",tag);
        intent.putExtra("value",value);
        intent.putExtra("CARIP",carip);
        startService(intent);
    }
}


