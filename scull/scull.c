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

#include "scull.h"
//module_param(variable, type, perm)

static char scull_name[]="scull";
static int scull_major = SCULL_MAJOR;
static int scull_minor = SCULL_MINOR;
static int scull_minor_count = SCULL_MINOR_COUNT;

struct cdev *my_cdev = NULL;
static dev_t ascull; /*设备号*/
//MKDEV(scull_major, scull_minor);		/*只是创建，没有注册到设备当中*/

static loff_t scull_llseek(struct file * f, loff_t pos, int p)
{
	return 0;	
}
static ssize_t scull_read(struct file * f, char __user * user_buff, size_t size, loff_t *pos)
{
	return 0;
}
static ssize_t scull_write(struct file * f, const char __user * user_buff, size_t size, loff_t *pos)
{
	return 0;
}
static long int scull_ioctl(struct file * in, unsigned int a, unsigned long b)
{
	return 0;
}
static int scull_open(struct inode * in, struct file * f)
{
	return 0;
}
static int scull_release(struct inode * in, struct file * f)
{
	return 0;
}

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.unlocked_ioctl = scull_ioctl,
	.open = scull_open,
	.release = scull_release,
};

static int device_number_init(void)
{

	int ret;
	if (scull_major)
	{
		ascull = MKDEV(scull_major, scull_minor);
		ret = register_chrdev_region(ascull, scull_minor_count, "scull");
	}
	else
	{
		/*register_chrdev_region用于静态分配设备号*/
		ret = alloc_chrdev_region(&ascull, MKDEV(0,0), scull_minor_count, "scull"); 
	
	}
	if(ret)
	{
		printk(KERN_ALERT "%s: can't regist device number %d\n", scull_name, ret);
		return -1;
	}
	
	printk("%s: major:%d, minor:%d\n", scull_name,  MAJOR(ascull), MINOR(ascull));
	
	
	return 0;
}

static int device_cdev_init(void)
{
	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &scull_fops);	/*添加ops*/
	my_cdev->owner = THIS_MODULE;	/**/
	return 0;
}

static int device_cdev_add_to_kernel(void)
{
	int ret;
	ret = cdev_add(my_cdev, ascull, 1);

	if(ret)
	{
		cdev_del(my_cdev);
	}
	return 0;
}

static int __init initialization_function(void)		/*__init 修饰符表示， 只能在初始化时调用， 不能调用两次*/
{
	printk(KERN_ALERT "%s: we are in initialization function \n",scull_name);
	device_number_init();
	device_cdev_init();
	device_cdev_add_to_kernel();		
	return 0;
}

static void __exit deinitialization_function(void)
{
	printk(KERN_ALERT "%s: deinitialization module exit\n",scull_name);
	if(my_cdev)cdev_del(my_cdev);
	unregister_chrdev_region(ascull, scull_minor_count);
}

	//module_init 函数是必须调用的
module_init(initialization_function)
module_exit(deinitialization_function)

MODULE_AUTHOR("Lyle");
MODULE_DESCRIPTION("scull module like ldd3's dscribe");
MODULE_LICENSE("GPL");
