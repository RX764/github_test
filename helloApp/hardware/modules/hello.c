#include <hardware/hardware.h>
#include <stdlib.h>
#include <fcntl.h>
#include <hardware/hello.h>
#include <stdio.h>
#define pr_err printf
struct hw_module_methods_t hello_method = {
        open : hello_dri_open
};

struct hello_module_t HAL_MODULE_INFO_SYM = {
        common : {
                tag : HARDWARE_MODULE_TAG,
                version_major : 1,
                version_minor : 0,
                id : "hello",
                name : "hello",
                methods : &hello_method
        }
};

int hello_get_val(struct hello_device_t *dev,int *val){
    read(dev->fd,val, sizeof(val));
    pr_err("jiangruxing:hello_get_val %d ",val);
    return val;
}

int hello_set_val(struct hello_device_t *dev,int val){
    write(dev->fd,&val, sizeof(val));
    pr_err("jiangruxing:hello_set_val %d",val);
    return 0;
}
int hello_close(struct hw_device_t* device){
    struct hello_device_t *dev = (struct hello_device_t *)device;
    close(dev->fd);
    free(dev);
    return 0;
}

int hello_dri_open(const struct hw_module_t* module, const char* id,
            struct hw_device_t** device){
    struct hello_device_t *dev = malloc(sizeof(struct hello_device_t));
    memset(dev,0, sizeof(struct hello_device_t));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = module;
    dev->common.close = hello_close;
    dev->get_val = hello_get_val;
    dev->set_val = hello_set_val;
    *device = dev;
    if((dev->fd = open("/dev/hello",O_RDWR))== -1){
        pr_err("jiangruxing:fail open /dev/hello");
    }
    return 0;
}
