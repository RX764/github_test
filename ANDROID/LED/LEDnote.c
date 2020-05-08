//
// Created by jrx on 2020/5/7.
//
/*
 * 端口配置寄存器GPMCON
 * 端口数据寄存器：GPMDAT
 * 端口上拉电路寄存器：GPMPUD     这三个寄存器每个都可以使用4字节，即一个int占用的空间
 *读写虚拟地址的函数：ioread32,iowrite32读写32位的数据，还有ioread16/8,iowrite16/8
 * 对应的较早的函数：readl,readb,readw，writel......
 */

/*
 * 在装载内核的时候，用户可以向模块传递参数，形式为"insmod 模块名 参数名 = 参数值"，如果不传递，参数使用模块内的默认值
为Linux驱动指定一个模块参数需要使用module_param(name,type,perm).参数名，参数类型,读/写权限
使用modul_param指定模块参数时，会在/sys/module目录下生成一个一个目录。
 module_param可以指定文件的访问权限。S——IRUGO表示所有用户可读不能修改。S——IRUGO|S_IWUSR表示所有用户可读可写
 S_IRWXUGO表示所用用户可以对文件读写执行。IWUGO所用用户只读(定义在linux/include/stat.h)
S_IRUSR：Permits the file's owner to read it.
S_IWUSR：Permits the file's owner to write to it.
S_IRGRP：Permits the file's group to read it.
S_IWGRP：Permits the file's group to write to it.

 module_param_array(name,type,nump,perm) nump:存储数组长度的变量的指针。
 第三个参数要使用指针类型的数据比如:@param_size
 指定数组类型的参数，逗号前后不能用空格
 */

/*
 * 创建设备文件步骤
 * 1.cdev_init初始cdev
 * 2.指定设备号
 * 3.cdev_add将字符设备添加到内核的字符设备数组中
 * 4.使用class_create宏创建struct_class
 * 5.使用device_create创建设备文件
 * 卸载步骤
 * void device_destroy(struct class *class,dev_t devt);
void class_destroy(struct class *cls);
void unregister_chardev_region(dev_t from ,unsigned count)

 eg:
#define DEVICE_NAME "dev_name"
#define DEVICE_COUNT 1
#define DEV_MAJOR 0
#define  DEV_MINOR 234
static int major = DEV_MAJOR;
static int minor = DEV_MINOR;
static dev_t dev_number;
static struct class  *leds_class =NULL;
static struct file_operations dev_fops =
        {
            .owner=THIS_MODULE,
            .unlocked_ioctl = leds_ioctl,
            .write = less_wwrite,
        };
static struct cdve leds_cdev;
//创建设备文件
static int leds_create_device(){
    int ret = 0;
    int err = 0;
    //第一步，初始化cdev,建立cdev和file_option的联系
    cdev_init(&leds_cdev,&dev_fops);
        //创建的设备属于当前模块
    leds_cdev.owner = THIS_MODULE;
    //第二步，注册设备号
    if(major > 0)
    {
        dev_number = MKDEV(major,minor);
        err = register_chrdev_region(dev_number,DEVICE_COUNT,DEVICE_NAME);
        if(err<0)
        {
            pr_err("-------------------------------------------------注册失败\n")
            return err;
        }
    } else
    {
        //10表示起始设备号
        err = alloc_chrdev_region(&leds_cdev.dev,10,DEVICE_COUNT,DEVICE_NAME);
        f(err<0)
        {
            pr_err("-------------------------------------------------注册失败\n")
            return err;
        }
    }
    //第三步
    ret = cdev_add(&leds_cdev,dev_number,DEVICE_COUNT);
    //第四步
    leds_class = class_create(THIS_MODULE,DEVICE_NAME);
    //第五步
    device_create(leds_class,NULL,dev_number,NULL,DEVICE_NAME);
    return ret;

 */


#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/device.h>
























