#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include "product.h"

extern int add(int x1 , int x2);

//初始化驱动
static int  main_init(void)
{
    int x1=20;
    int x2 = 68;
    pr_err("-----------------------------------------------multi_file_driver_main_init_success\n");
    pr_err("%d + %d = %d\n",x1,x2,add(x1,x2));
    pr_err("-----------------------------------------------prodect Name: %s\n",get_product_name());
    return 0;
}
static void  main_exit()
{
    pr_err("-----------------------------------------------multi_file_driver_main_exit_success\n");
}
module_init(main_init);
module_exit(main_exit);
MODULE_LICENSE("GPL");
