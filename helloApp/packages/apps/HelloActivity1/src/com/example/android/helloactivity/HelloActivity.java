
package com.example.android.helloactivity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.os.IHelloService;
import android.widget.Toast;
import android.os.ServiceManager;
import android.util.Log;



public class HelloActivity extends Activity {
   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.hello_activity);
	    final IHelloService testService = IHelloService.Stub.asInterface(ServiceManager.getService("hello"));
        if (testService == null) {
            Log.e("testService", "Get Hello Service falied.");
            Toast.makeText(HelloActivity.this, "Get Hello Service falied.", Toast.LENGTH_SHORT).show();
            return;
        } 	
		Log.d("testService", "testService : " + testService.toString());
		
        final EditText et_set = (EditText) findViewById(R.id.et_set);
        final TextView tv_get = (TextView) findViewById(R.id.tv_test);
        findViewById(R.id.bt_getval)
                .setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        try {
                            Toast.makeText(HelloActivity.this, "c:"+testService.getVal(), Toast.LENGTH_SHORT).show();
                            tv_get.setText(testService.getVal()+"");
                        } catch (Throwable e) {
                            Toast.makeText(HelloActivity.this, "b:"+e, Toast.LENGTH_SHORT).show();
                            e.printStackTrace();
                        }
                    }
                });

        findViewById(R.id.bt_setval).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    testService.setVal(Integer.parseInt(et_set.getText().toString()));
                    Toast.makeText(HelloActivity.this, "d:"+Integer.parseInt(et_set.getText().toString()), Toast.LENGTH_SHORT).show();
                } catch (Throwable e) {
                    Toast.makeText(HelloActivity.this, "e:"+e, Toast.LENGTH_SHORT).show();
                    e.printStackTrace();
                }
            }
        });


    }


    
}

