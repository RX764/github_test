#include <jni.h>
#include <JNIHelp.h>
#include <hardware/hardware.h>
#include <android_runtime/AndroidRuntime.h>
#include <hardware/hello.h>
#include <utils/Log.h>
namespace android{

    static jint getVal(JNIEnv *env,jobject obj,jint ptr){
       struct hello_device_t *dev = (struct hello_device_t *)(jlong)ptr;
        int val = 0;
        dev->get_val(dev,&val);
        return val;
    }

   static void setVal(JNIEnv *env,jobject obj,jint ptr,jint val){
       struct hello_device_t *dev = (struct hello_device_t *)(jlong)ptr;
	   dev->set_val(dev,val);
    }

   static jint hello_init(JNIEnv *env,jobject obj){
        struct hello_module_t *module;
        struct hello_device_t *device;

        hw_get_module("hello",(const struct hw_module_t **)&module);
        module->common.methods->open(&(module->common),"hello",(struct hw_device_t **)&device);
       // return (jint)device;
		  return (jint)(jlong)device;
    }

    static const JNINativeMethod method_table[] = {
            {"init_hello","()I",(void *)hello_init},
            {"getVal_native","(I)I",(void *)getVal},
            {"setVal_native","(II)V",(void *)setVal}
    };

    int register_hello_Service(JNIEnv *env){
        jniRegisterNativeMethods(env,"com/android/server/HelloService",method_table,NELEM(method_table));
        return 0;
    }
}

