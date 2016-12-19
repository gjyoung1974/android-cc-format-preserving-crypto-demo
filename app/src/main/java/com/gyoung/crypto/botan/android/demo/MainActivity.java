package com.gyoung.crypto.botan.android.demo;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;


import com.gyoung.crypto.botan.android.demo.R;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("crystax");
        System.loadLibrary("test-botan");
    }

    //bring in our JNI Method
   public native String stringFromJNI(String ccid, String passwd);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        AppCompatButton fab = (AppCompatButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                TextView ccid_field = (TextView) findViewById(R.id.ccid);
                CharSequence ccid = ccid_field.getText().toString();

                TextView passwd_field = (TextView) findViewById(R.id.passwd);
                CharSequence passwd = passwd_field.getText().toString();

                String s_result = stringFromJNI(ccid.toString(), passwd.toString());

                TextView field = (TextView) findViewById(R.id.text);
                field.setText(s_result);
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
