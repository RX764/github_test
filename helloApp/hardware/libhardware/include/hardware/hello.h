#ifndef ANDROIDDRIVER_HELLO_H
#define ANDROIDDRIVER_HELLO_H
#include <hardware/hardware.h>

struct hello_module_t {
    struct hw_module_t common;

};

struct hello_device_t {

    struct hw_device_t common;
    int fd;
    int (*get_val)(struct hello_device_t *dev,int *val);
    int (*set_val)(struct hello_device_t *dev,int val);
};

int hello_dri_open(const struct hw_module_t* module, const char* id,

              struct hw_device_t** device);

#endif
