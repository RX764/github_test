#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

struct my_hello_dev{
    char val;
    struct cdev cv;
};

dev_t devNumber;
struct my_hello_dev *my_dev;

int hello_open (struct inode *node, struct file *filp){
    struct my_hello_dev *n_cdev = container_of(node->i_cdev,struct my_hello_dev,cv);
    filp->private_data = n_cdev;
    return 0;
}

int hello_release (struct inode *node, struct file *filp){
    filp->private_data = NULL;
    return 0;
}

ssize_t hello_read (struct file *filp, char __user *buf, size_t len, loff_t *pos){
    struct my_hello_dev *n_cdev = (struct my_hello_dev *)filp->private_data;
    int err;
	pr_err("jiang:hello read");
    if(copy_to_user(buf,&n_cdev->val, sizeof(n_cdev->val))){
        err = -EFAULT;
        return err;
    }
    return sizeof(n_cdev->val);

}

ssize_t hello_write (struct file *filp, const char __user *buf, size_t len, loff_t *pos){
    struct my_hello_dev *n_cdev = (struct my_hello_dev *)filp->private_data;
    int err;
	pr_err("jiang:hello write");
    if(copy_from_user(&n_cdev->val,buf,len)){
        err = -EFAULT;
        return err;
    }
    return sizeof(n_cdev->val);
}

struct file_operations hello_op = {
    .owner = THIS_MODULE,
    .open  = hello_open,
    .release = hello_release,
    .write = hello_write,
    .read = hello_read
};




static int hello_init(void){
	 struct class * cls =NULL;	

    int err = alloc_chrdev_region(&devNumber,0,1,"hello");
    if(err){
		pr_err("jiang:获取设备号失败");
        return err;
    }
    my_dev = kmalloc(sizeof(struct my_hello_dev),GFP_KERNEL);
    cdev_init(&my_dev->cv,&hello_op);
    err = cdev_add(&my_dev->cv,devNumber,1);
    if(err){
        pr_err("jiang:添加设备失败");
        return err;
    }

    my_dev->val = '8';
    cls = class_create(THIS_MODULE,"hello");
    device_create(cls,NULL,devNumber,NULL,"hello");

    return 0;
}

static void hello_exit(void){
    unregister_chrdev_region(devNumber,1);
    kfree(my_dev);
    cdev_del(&my_dev->cv);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
