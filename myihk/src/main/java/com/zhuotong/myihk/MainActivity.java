package com.zhuotong.myihk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.alibaba.fastjson.JSONObject;
import com.craft.nativeJNI.NativeHelper;

public class MainActivity extends AppCompatActivity {

    static {
        try {
            System.loadLibrary("CR");
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }




    public static void add(){
        Log.d("YYYYY","XXXXXXXXXXXXXXXXXX");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button bt = findViewById(R.id.bt);
        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NativeHelper.startMethodTrace();
            }
        });

    }

}
