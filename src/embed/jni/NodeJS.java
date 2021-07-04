package com.lufinkey.embed;

public class NodeJS {
	static {
		loadLibraries();
	}

	public static void loadLibraries() {
		System.loadLibrary("node");
		System.loadLibrary("NodeJSEmbed");
	}

	public native static void start();


	public interface ProcessEventListener {
		void onNodeJSProcessWillStart(String[] args);
		void onNodeJSProcessDidStart();
		void onNodeJSProcessWillEnd();
		void onNodeJSProcessDidEnd(int exitCode);
	}

	public native static void addProcessEventListener(ProcessEventListener listener);
	public native static void removeProcessEventListener(ProcessEventListener listener);
}
