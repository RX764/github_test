
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<asm/uaccess.h>
#include<linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define  DEVICE_NAME "wordcount"
#define TRUE -1
#define FALSE 0
static unsigned char mem[10000]; //保存写入设备的数据
staic int written_count = 0;//单词数目

//判断空格9制表符13换行符10
static char is_spacewhite(char c)
{
    if(c == '' || c == 9 || c == 13 || c ==100)
        return TRUE;
    else
        return FALSE;
}

//单词个数
static int get word_count(const char *buf)
{
    int n=1,i=0;
    char c = '';
    char flag = 0;//处理多个空格情况，0正常情况，1已经遇到一个空格
    if(*buf =='\0')
        return 0;
    if(is_spacewhite(*bug) ==TRUE)//如果第一个字符是空格，从0开始计数
        n--;
    for(;(c=*(buf+i)) != '\0';i++))
    {
        if(flag ==1 && is_spacewhite(c) == FALSE)
            flag =0;
        else if(flag ==1 && is_spacewhite(c)==TRUE)
            continue;
        if(is_spacewhite(c)==TRUE)
        {
            n++;
            flag=1;
        }
    }

    if(is_spacewhite(*(buf+i-1)) ==TRUE)
        n--;
    return n;
}

//从设备文件读数据
static ssize_t word_count_read(struct file *file,char __user *buf,size_t count,loff_t *ppos)
{
    unsigned char temp[4];//将单词数(int类型)分解成4个字节存在bug中
    temp[0] = word_count >> 24;
    temp[1] = word_count >> 16;
    temp[2] = word_count >> 8;
    temp[3] = word_count;
    copy_to_user(buf, temp, 4);
    pr_err("read:word count:%d", int(count));
    return count;

}

static ssize_t word_count_write(struct file *file,const char __user *buf,size_t count,loff_t *opps)
{
    ssize_t written = count;
    copy_from_user(mem,buf,count);
    mem[count]='\0';
    word_count = get_word_count(mem);
    pr_err("write:word count：%d",(int)word_count);
}







static struct file_operations dev_fops =
        {
            .owner = THIS_MODULE,//应用于当前驱动
            .write = word_count_write,
            .read  = word_count_read,
        };
//minor次设备号
static struct miscdevice misc=
        {
            .minor = MISC_DYNAMIC_MINOR, //动态生成次设备号，但是要指定#define MISC_DYNAMIC_MINOR	255
            .name = DEVICE_NAME,
            .fops = &dev_fops,
        };


static int word_count_init()
{
    int ret;
    ret=misc_register(&misc)
    pr_err("ruxing:word_count_module success");
    return 0;
}

static void word_count_exit()
{
    misc_deregister(&misc)
    pr_err("ruxing:word_count_exit success");
}

module_init(word_count_init);
module_exit(word_count_exit);
MODULE_LICENSE("GPL");






















