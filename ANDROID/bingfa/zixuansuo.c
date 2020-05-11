//
// Created by jrx on 2020/5/11.
//
//使用读写自旋锁保护临界区代码
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
#include <linux/delay.h>
#include <linux/random.h>

#define DEVICE_NAME "rw_lock"
static char *data = "read\n";
static DEFINE_RWLOCK(rwlock);
static  DEFINE_LOCK(lock);
static  char *lock_type = "rw";//rw读写自旋锁, spin自旋锁

static ssize_t demo_read(struct file *file,char __user buf,ssize_t count,loff_t *ppos)
{
    int size = strlen(data);
    struct timeval tv;
    if(copy_to_user(buf,(void*)data,size))
    {
        return -EINVAL;
    }
    do_gettimeofday(&tv);
    pr_err("-------------------------------------read start sec:%ld\n",tv.tv_sec);
    if(strcmp(local_type,"rw") == 0)
        read_lock(&rwlock);
    else if(strcmp(lock_type,"spin") == 0)
        spin_lock(&lock);
    do_gettimeofday(&tv);
    pr_err("-------------------------------------read end sec:%ld\n",tv.tv_sec);
    mdelay(5000);
    if(strcmp(local_type,"rw") == 0)
        read_unlock(&rwlock);
    else if(strcmp(lock_type,"spin") == 0)
        spin_lock(&lock);
    return size;
}


static ssize_t demo_write(struct file *file,char __user buf,ssize_t count,loff_t *ppos)
{
    truct timeval tv;
    do_gettimeofday(&tv);
    pr_err("-------------------------------------read start sec:%ld\n",tv.tv_sec);
    if(strcmp(local_type,"rw") == 0)
        write_lock(&rwlock);
    else if(strcmp(lock_type,"spin") == 0)
        spin_lock(&lock);
    do_gettimeofday(&tv);
    pr_err("-------------------------------------read end sec:%ld\n",tv.tv_sec);
    mdelay(5000);
    if(strcmp(local_type,"rw") == 0)
        read_unlock(&rwlock);
    else if(strcmp(lock_type,"spin") == 0)
        spin_unlock(&lock);
    return count;

}

//.................




//使用rwlock机制端代码
static enum audit_state audit_filter_task(struct task_struct *tsk)
{

}





/*
 自旋锁的4种操作
 1.定义自旋锁变量 (2种方式)
    (1)spinlock_t lock;
    (2)DEFINE_SPINLOCK(lock);
 2.初始化自旋锁变量
    spin_lock_init(&lock);
 3.获取自旋锁
    spin_lock(&lock)
        如果获取锁成功，就会立即返回，否则spin_lock函数就会被阻塞直到获取锁
        如果想不管成功与否都返回，用：spin_trylock(&lock),c成功返回非0
 4.释放锁
    spin_unlock(&lock);
其他函数：
    int spin_lock_irq(&lock)  获取自旋锁并禁止中断
    void spin_unlock_irq(&lock)放锁并允许中断
    .........

读写自旋锁：
    自旋锁不关心临界区域如何操作，只是只允许一个执行单元获取自旋锁。在并发访问共享资源时，过个执行单元同时读取是不会有问题的。所以
    衍生了读写自旋锁。读写不能同时进行。
    使用方式：
        1.定义变量
            rwlock_t lock  或者  DEFINE_RWLOCK(lock)
        2.初始化：
            rwlock_init(&lock)
        3.获取
            read_lock(&lock)  ----->read_trylock(&lock)
            write_lock(&lock) ------>write_trylock(&lock)
            注意：不能同时申请读和写，否则会造成死锁
        4.释放锁
            read_unlock(&lock)
            write_unlock(&lock)
==================================================================================
 顺序锁：与读写自旋锁类似，不过写锁权限更高。特点是读数据的时候可以写数据
 顺序锁要定义seqlock_t变量。释放和获取写锁时，sequence都会+1，
    seqlock_t lock;
    seqlock_init(&lock);
    write_seqlock(&lock)
    write_sequnlock(&lock)

读取共享资源：
    unsigned seq;
    do
    {
        seq = read_seqbegin(&seqlock);---------> read_seqbegin获取顺序号，即sseqlock_t.sequence
        //临界区代码。。。。
    }while(read_seqretry(&seqlock,seq)); ----->判断执行临界区的过程中顺序号是否变化




RCU机制：（可看做rw_lock的扩展）
读操作：直接对共享资源进行访问，RCU上下文是不可抢占的，所以可以用read_rcu_lock，该函数作用是停止抢占
写操作：写共享资源前对修改的旧数据备份。更新旧数据采用rcu_assign_pointer宏。这个过程是旧数据的指针指向新数据，并不是覆盖
    使用RCU机制同时只能有一个线程写共享数据。
RCU机制中存在一个垃圾回收的daemon(后台线程)，当共享资源被修改后，可以采用它实现旧数据资源回收。
RCU的操作
 读锁定：
    static inline void rcu_read_lock(void)
    static inline void rcu_read_lock_bh(void)
读解锁：
    static inline void rcu_read_unlock(void)
    static inline void rcu_read_unlock_bh(void)
同步RCU
    synchronize_rcu():由RCU写线程调用，它将阻塞线程。
挂接回调函数
 call_rcu函数由RCU写线程调用，不会使写线程阻塞，所以可以在中断上下文或软中断中使用。









 */