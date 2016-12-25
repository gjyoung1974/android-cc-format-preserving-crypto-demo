package com.gyoung.crypto.botan.android.demo;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;

import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.content.Intent;
import android.content.Context;
import android.widget.TextView;

public class DecryptActivity extends AppCompatActivity {

    static {
        System.loadLibrary("crystax");
        System.loadLibrary("test-botan");
    }

    //bring in our JNI Method
   public native String DEcryptCCString(String ccid, String passwd);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decrypt);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab1 = (FloatingActionButton) findViewById(R.id.fab);
        final Context context = this;

        fab1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent myIntent = new Intent(context, MainActivity.class);
                context.startActivity(myIntent);

            }
        });

        AppCompatButton appButton = (AppCompatButton) findViewById(R.id.bDecrypt);
        appButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                TextView ccid_field = (TextView) findViewById(R.id.ccid);
                CharSequence ccid = ccid_field.getText().toString();

                TextView passwd_field = (TextView) findViewById(R.id.passwd);
                CharSequence passwd = passwd_field.getText().toString();

                String s_result = DEcryptCCString(ccid.toString(), passwd.toString());

                TextView field = (TextView) findViewById(R.id.text);
                field.setText(s_result);
            }
        });

    }

}
