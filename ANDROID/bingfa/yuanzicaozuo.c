//
// Created by jrx on 2020/5/10.
//
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/uaccess.h>
#include<asm/atomic.h>
#include<linux/types.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

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
        atomic_inc(&int_atomic_avalible);
    }
    return 0;
}

static struct file_operations dev_fops =
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

static  int  atomic_init()
{
    int ret = misc_register(&misc);
    pr_err("--------------------------------------------------------------------------\n");
    pr_err("atomic_init_success\n");
    pr_err("--------------------------------------------------------------------------\n");
    return ret;
}

static void  atomic_exit()
{
    pr_err("--------------------------------------------------------------------------\n");
    pr_err("atomic_exit_success\n");
    pr_err("--------------------------------------------------------------------------\n");
    misc_deregister(&misc);
}
module_init(atomic_init);
module_exit(atomic_exit);
module_param(atom,int,S_IRUSR|S_IWUSR);
MODULE_LICENSE("GPL")



//test.c
#include <stdio.h>
#include <errno.h>
int main()
{
    int handler = open("/dev/atomic",0);
    printf("handler:%d\n",handler);
    if(handler>0)
    {
        getchar();
        close(handler);
    } else{
        printf("errno:%d\n",errno);
    }
}


/*
将对int的操作变成原子操作，需要依靠atomic_t数据类型。linux/types.h中定义：
    typedef struct{
        volatile int counter; //volatile修饰符告诉gcc不要对该类型数据进行优化处理，即对它的访问都是对
          //内存的访问，而不是对寄存器的访问。即要读，必须重新找个寄存器载入该参数，而不是直接利用该参数
          //此刻在其他高速寄存器中的备份。
        }atomic_t;
将变量类型定义为atomic_t，并通过一些函数操作来操作atomic_t.counter变量，就能保证是原子的。
定义atomic_t类型和普通变量一样，只是需要ATOMIC_INIT宏初始化：
    atomic_t v;
    atomic_t n = ATOMIC_T(1)
操作atomic_t类型的变量需要使用一些函数，比如初始化，加减法
    atomic_t n;
    atomic_set(&n,2);  =2
    atomic_add(5,&n);  +5
    atomic_dec(&n)    -1
    atomic_inc(&n)    +1
    atomic_sub(5,&n)  -5
    printk("%d“，atomic_read(&n));
    atomic_add_return(2,&n) +2返回
    atomic_add_and_test(2,&n) 如果等于0，返回非0值(真)。
上面是32位的，下面是64位的。如果不确定，请用32位！
 typedef struct{
    u64 _aligned(8) counter;//long long在32和64都占8字节
 }atomic64_t


 位原子操作：操作类型是unsigned long 64位是64bit，32位32bit
 作用：将指定位置0，置1
unsignde long value=0;
 set_bit(i,&value),设置第i位为1.
 printk("%d",value)
 change_bit(0,&value)//改变第0位值
 cler_bit(i,&value)//第i位置0
 test_and_set_bit(i,&value)//将i位置1，如果之前就是1，就返回1
 test_bit(i,&value)如果第i位为1，返回1
 */