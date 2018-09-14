package com.omegastar.carem;

import android.app.IntentService;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;



public class carEmBackgroundTask extends IntentService {

    public boolean dd; //debug detail
    public boolean d;  //debug

    public carEmBackgroundTask() {
        super("HelloIntentService");
        dd=false;
        d=true;
        if (dd) Log.d("CAREM","Starting");
    }


    /**
     * The IntentService calls this method from the default worker thread with
     * the intent that started the service. When this method returns, IntentService
     * stops the service, as appropriate.
     */
    @Override
    protected void onHandleIntent(Intent intent) {
        if (dd) Log.d("CAREM", "onHandleIntent-enter");
        URL url;
        HttpURLConnection urlConnection = null;
        String result="KO";
        try {
            Bundle b = intent.getExtras();

            String rest="";
            String tag = "";
            String value = "";

            String carip = b.getString("CARIP");
            rest = b.getString("rest");
            try {
                tag = b.getString("tag");
                value = b.getString("value");
            }  catch (Exception e) {
                e.printStackTrace();
                if (d) Log.d("CAREM-BG Exception", "no tar or value");
            }

            url = new URL("http://" + carip + ":5001/api/v1.0/ce/" + rest);
            urlConnection = (HttpURLConnection) url.openConnection();

            if (tag != null && !tag.isEmpty()) {
               // POST
                urlConnection.setDoOutput(true);
                urlConnection.setDoInput(true);
                urlConnection.setInstanceFollowRedirects(false);
                urlConnection.setRequestMethod("POST");
                urlConnection.setRequestProperty("Content-Type", "application/json");
                //urlConnection.setRequestProperty("Content-Length", "" + Integer.toString(otherParametersUrServiceNeed.getBytes().length));
                urlConnection.setUseCaches (false);

                urlConnection.connect();
                JSONObject jsonParam = new JSONObject();
                jsonParam.put(tag, value);
                if (d) Log.d("CAREM", "POST CAR IP:"+carip+" " + rest + " " + tag + " " + value );

                DataOutputStream printout = new DataOutputStream(urlConnection.getOutputStream());
                printout.writeBytes(jsonParam.toString());
                printout.flush ();
                printout.close ();
            }
            else {
                if (d) Log.d("CAREM", "GET CAR IP:"+carip+" " + rest);
            }

            StringBuilder translateResult = new StringBuilder(200);
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));
                char[] buffer = new char[8192];
                int charsRead;
                while ((charsRead = in.read
                        (buffer)) > 0) {
                    translateResult.append(buffer, 0, charsRead);
                }
                if (dd) Log.d("CAREM","response:"+ translateResult.toString());
            } catch (Exception e) {
                if (d) Log.d("CAREM", "Exception" + e.getLocalizedMessage());
            }
            //Process json
            try {
                JSONObject object = new JSONObject(translateResult.toString());
                if (dd) Log.d("CAREM","response json:"+ object.toString());
                result = object.getString("result");
                if (d) Log.d("CAREM","response result:"+ result);
            } catch (Exception e) {
                if (d) Log.d("CAREM","Exception "+ e.getLocalizedMessage());
            }
        } catch (Exception e) {
            e.printStackTrace();
            if (d) Log.d("CAREM", "onHandleIntent-exit-exception");
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
        }
        sendResultUI(result);
        if (dd) Log.d("CAREM", "onHandleIntent-exit");
    }

    static final public String CAREM_RESULT = "com.carem.REQUEST_PROCESSED";
    static final public String CAREM_MESSAGE = "com.carem.MESSAGE";
    public void sendResultUI(String message) {
        LocalBroadcastManager broadcaster = LocalBroadcastManager.getInstance(this);
        Intent intent = new Intent(CAREM_RESULT);
        if(message != null)
            intent.putExtra(CAREM_MESSAGE, message);
        broadcaster.sendBroadcast(intent);
    }
}



