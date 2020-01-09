/*
 * step by step build scull
 */

#include <linux/module.h>	/*THIS_MODULE. 必须的，包含可加载模块的很多符号和函数定义 */
#include <linux/init.h>		/*用来调用函数指定模块初始化函数和清理函数*/
#include <linux/types.h>	/*dev_t*/
#include <linux/kdev_t.h>	/*MAJOR() MINOR()*/
#include <linux/moduleparam.h>	/*module_param()*/
#include <linux/fs.h>		/*file_operations*/

//module_param(variable, type, perm)


static int scull_major = 111;
static int scull_minor = 222;


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


static int do_actual_work(void)
{
	dev_t ascull = MKDEV(scull_major, scull_minor);
	printk("major:%d, minor:%d\n", MAJOR(ascull), MINOR(ascull));
	return 0;
}

static int __init initialization_function(void)		/*__init 修饰符表示， 只能在初始化时调用， 不能调用两次*/
{
	printk(KERN_ALERT "we are in initialization function \n");
	do_actual_work();
	return 0;
}

static void __exit deinitialization_function(void)
{
	printk(KERN_ALERT "deinitialization module exiti\n");
}

	//module_init 函数是必须调用的
module_init(initialization_function)
module_exit(deinitialization_function)

MODULE_AUTHOR("Lyle");
MODULE_DESCRIPTION("scull module like ldd3's dscribe");
MODULE_LICENSE("GPL");
