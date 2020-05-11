#include <linux/module.h>
#include <linux/init.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
/*
#include <linux/types.h>
#include <linux/proc_fs.h>
//#include <linux/device.h>
#include <asm/uaccess.h>
*/

#define DEVICE_NAME "atomic"

//0:多个进程可以同时打开。非0:同时只能有一个进程打开设备文件
static int atom = 0;

static atomic_t int_atomic_avaliable = ATOMIC_INIT(1);

static int atomic_open(struct inode *node,struct file *file)
{
    if(atom)
    {
        if(!atomic_dec_and_test(&int_atomic_avaliable))//为1则表示已经被其他进程打开
        {
            atomic_inc(&int_atomic_avaliable);
            return -EBUSY;
        }
    }
    return 0;
}

static int atomic_release(struct inode *inode,struct file *file)
{                       
    if(atom)
    {
        atomic_inc(&int_atomic_avaliable);
    }
    return 0;
}

static struct file_operations dev_fops=
        {
            .owner = THIS_MODULE,
            .open = atomic_open,
            .release = atomic_release,
        };
static  struct miscdevice misc=
        {
            .minor = MISC_DYNAMIC_MINOR,
            .name = DEVICE_NAME,
            .fops=&dev_fops,
        };

static  int __init atomic_init()
{
    int ret = misc_register(&misc);
    pr_err("--------------------------------------------------------------------------\n");
    pr_err("atomic_init_success\n");
    pr_err("--------------------------------------------------------------------------\n");
    return ret;
}

static void  __exit atomic_exit()
{
    pr_err("--------------------------------------------------------------------------\n");
    pr_err("atomic_exit_success\n");
    pr_err("--------------------------------------------------------------------------\n");
    misc_deregister(&misc);
}
module_init(atomic_init);
module_exit(atomic_exit);
module_param(atom,int,S_IRUSR|S_IWUSR);
MODULE_LICENSE("GPL");

