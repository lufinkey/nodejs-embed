package com.lufinkey.nodejsembedtest;

import com.lufinkey.embed.NodeJS;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements NodeJS.ProcessEventListener {

	// Used to load the 'native-lib' library on application startup.
	static {
		NodeJS.loadLibraries();
		System.loadLibrary("TestApp");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// Example of a call to a native method
		TextView tv = findViewById(R.id.sample_text);
		tv.setText(stringFromJNI());

		NodeJS.addProcessEventListener(this);
		NodeJS.start();
	}

	/**
	 * A native method that is implemented by the 'native-lib' native library,
	 * which is packaged with this application.
	 */
	public native String stringFromJNI();

	public void onNodeJSProcessWillStart(String[] args) {
		System.out.println("onNodeJSProcessWillStart: " + args.toString());
	}

	public void onNodeJSProcessDidStart() {
		System.out.println("onNodeJSProcessDidStart");
	}

	public void onNodeJSProcessWillEnd() {
		System.out.println("onNodeJSProcessWillEnd");
	}

	public void onNodeJSProcessDidEnd(int exitCode) {
		System.out.println("onNodeJSProcessDidEnd: "+exitCode);
	}
}
