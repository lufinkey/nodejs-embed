//
//  NodeJS_jni.cpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/16/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <jni.h>

namespace embed::nodejs {
	class ScopedJNIEnv {
	public:
		ScopedJNIEnv(JavaVM*);
		ScopedJNIEnv(const ScopedJNIEnv&) = delete;
		ScopedJNIEnv& operator=(const ScopedJNIEnv&) = delete;
		~ScopedJNIEnv();

		JNIEnv* getEnv() const;

	private:
		JavaVM* vm;
		JNIEnv* env;
		bool attachedToThread;
	};
}
