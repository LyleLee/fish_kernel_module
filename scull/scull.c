/*
* step by step build scull
*/

#include <linux/module.h>	/*THIS_MODULE. 必须的，包含可加载模块的很多符号和函数定义 */
#include <linux/init.h>		/*用来调用函数指定模块初始化函数和清理函数*/
#include <linux/types.h>	/*dev_t*/
#include <linux/kdev_t.h>	/*MAJOR() MINOR()*/
#include <linux/moduleparam.h>	/*module_param()*/
#include <linux/fs.h>		/*file_operations register_chrdev_region() alloc_chrdev_region() unregister_chrdev_region*/
#include <linux/cdev.h>
#include <linux/slab.h>		/*kmalloc() kfree()*/

#include "scull.h"

struct scull_dev* scull_devices;

static char scull_name[]   ="scull";
static char scull_major    = 0;
static int  scull_many     = 4;

static loff_t scull_llseek(struct file * filp, loff_t pos, int p)
{
    printk(KERN_NOTICE "scull: is being llseek\n");
    return 0;
}
static ssize_t scull_read(struct file * filp, char __user * user_buff, size_t size, loff_t *pos)
{
    printk(KERN_NOTICE "scull: is being read\n");
    return 0;
}
static ssize_t scull_write(struct file * filp, const char __user * user_buff, size_t size, loff_t *pos)
{
    printk(KERN_NOTICE "scull: is being write\n");
    return 0;
}
static long int scull_ioctl(struct file * in, unsigned int a, unsigned long b)
{
    return 0;
}
static int scull_open(struct inode * in, struct file * f)
{
    printk(KERN_NOTICE "scull: is being open\n");
    return 0;
}
static int scull_release(struct inode * in, struct file * f)
{
    printk(KERN_NOTICE "scull: is being release\n");
    return 0;
}

struct file_operations scull_fops = {
    .owner          = THIS_MODULE,
    .llseek         = scull_llseek,
    .read           = scull_read,
    .write          = scull_write,
    .unlocked_ioctl = scull_ioctl,
    .open           = scull_open,
    .release        = scull_release,
};
/*初始化代表设备结构体*/
static void scull_setup_cdev(struct scull_dev* onedev, int index)
{
    int err;
    cdev_init(&onedev->cdev, &scull_fops);
    onedev->cdev.owner = THIS_MODULE;
    //printk(KERN_ALERT "scull: scull_fops address: %p\n", &scull_fops);
    //printk(KERN_ALERT "scull: Char devices fops address: %p\n", onedev->cdev.ops);

    //字符设备添加到系统，cdev的指针， 设备编号(含次设备的其实编号)， 次设备号数量
    err = cdev_add(&onedev->cdev, MKDEV(scull_major, index), 1);
    if(err)
        printk(KERN_NOTICE "Error %d adding scull %d %d", err, scull_major, index);
}

/*__init 修饰符表示， 只能在初始化时调用， 不能调用两次*/
static int __init initialization_function(void)
{
    int ret, i;
    dev_t dev_num = 0;

    printk(KERN_ALERT "%s: we are in initialization function \n", scull_name);

    /*用户已经有指定，所以这里直接请求kernel静态注册scull的设备号。在/proc/devices可以查询*/
    if (scull_major)
    {
        dev_num = MKDEV(scull_major, 0);
        ret = register_chrdev_region(MKDEV(scull_major, 0), scull_many, "scull");
    }
    /*不知道用哪个设备号，请求系统分配*/
    else
    {
        /*register_chrdev_region用于静态分配设备号*/
        ret = alloc_chrdev_region(&dev_num, 0, scull_many, "scull");
    }
    if(ret)
    {
        printk(KERN_ALERT "%s: can't regist device number %d\n", scull_name, ret);
        return -1;
    }

    scull_major = MAJOR(dev_num);

    printk("%s: major:%d, minor:%d\n", scull_name,  MAJOR(dev_num), MINOR(dev_num));

    scull_devices = kmalloc(sizeof(struct scull_dev) * scull_many, GFP_KERNEL);

    if(!scull_devices){
        ret = -ENOMEM;
        goto fail;
    }

    memset(scull_devices, 0, sizeof(struct scull_dev) * scull_many);

    for(i = 0; i < scull_many; i++)
    {
        scull_setup_cdev(&scull_devices[i], i);
    }

    return 0;

fail:
    return ret;
}

static void __exit deinitialization_function(void)
{
    int i;
    printk(KERN_ALERT "%s: deinitialization module exit\n",scull_name);
    if(scull_devices){
        for(i = 0; i < scull_many; i++){
            cdev_del(&scull_devices[i].cdev);//cdev_add()
        }
        //kfree(scull_devices);
    }
    unregister_chrdev_region(MKDEV(scull_major,0), scull_many);
}

//module_init 函数是必须调用的
module_init(initialization_function)
module_exit(deinitialization_function)

MODULE_AUTHOR("Lyle");
MODULE_DESCRIPTION("scull module like ldd3's dscribe");
MODULE_LICENSE("GPL");
