#ifndef NATIVE_HANDLE_H_
#define NATIVE_HANDLE_H_

#include <jni.h>

class NativeHandle{
public:

	static jfieldID getHandleField(JNIEnv *env, jobject obj)
	{
		jclass c = env->GetObjectClass(obj);
		// J is the type signature for long:
		return env->GetFieldID(c, "nativeHandle", "J");
	}

	template <typename T>
	static T *getHandle(JNIEnv *env, jobject obj)
	{
		jlong handle = env->GetLongField(obj, getHandleField(env, obj));
		return reinterpret_cast<T *>(handle);
	}

	template <typename T>
	static void setHandle(JNIEnv *env, jobject obj, T *t)
	{
		jlong handle = reinterpret_cast<jlong>(t);
		env->SetLongField(obj, getHandleField(env, obj), handle);
	}

};


#endif /* NATIVE_HANDLE_H_ */
