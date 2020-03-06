/*
* step by step build scull
*/

#include <linux/module.h>	/*THIS_MODULE. 必须的，包含可加载模块的很多符号和函数定义 */
#include <linux/init.h>		/*用来调用函数指定模块初始化函数和清理函数*/
#include <linux/slab.h>		/*kmalloc() kfree()*/
#include <linux/proc_fs.h>
#include <linux/uaccess.h>  /*copy_from_user, copy_to_user*/

#define MSG     100
#define MSGSIZE 1000

static char modname[] = "useproc";
static char kernel_buffer[MSG][MSGSIZE] = {0};
static ssize_t itr = 0;

/** write function from file_operations is
 *  ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
*/
static ssize_t my_proc_write(struct file * filep, const char __user *user_buffer, size_t count, loff_t *pos){
    if(count > MSGSIZE) return -EFAULT;
    if(itr > MSG-1){
        printk(KERN_INFO "kernel memory cache is full\n");
        return 0;
    }

    if(copy_from_user(kernel_buffer[itr], user_buffer, count)){
        return -EFAULT;
    }
    kernel_buffer[itr][count-1] = '\0';
    itr+=1;

    printk(KERN_INFO "msg has been save: %s\n", kernel_buffer[itr-1]);

    return count;

}
ssize_t my_proc_read(struct file *filp,char *user_buffer, size_t count, loff_t *offp ){
    
    int err;
    printk(KERN_INFO "read argument: %p, %p, %ld, %d\n", filp, user_buffer, count, (int)(*offp));
    
    if(itr <=0 ){
        printk(KERN_INFO "the kernel cache is empty\n");
        return 0;
    }

    if(count > MSGSIZE){
        printk(KERN_INFO "earch message should have only %d byte\n", MSGSIZE);
        return -EFAULT;
    }
    if(count == 0){
        printk(KERN_INFO "you are requesting 0 byte\n");
        return count;
    }

    err = copy_to_user(user_buffer, kernel_buffer[itr-1], count);
    
    printk(KERN_INFO "read data:%s\n", kernel_buffer[itr-1]);
    return count;
}

struct file_operations proc_fops = {
    .read = my_proc_read,
    .write = my_proc_write,
};


static int __init initialization_function(void){
    struct proc_dir_entry *ret = NULL;
    printk("%s: installing module\n", modname);
    ret = proc_create_data(modname, 0666, NULL, &proc_fops, NULL);
    if(!ret) printk("useproc error\n");
    return 0;
}
static void __exit deinitialization_function(void){
    remove_proc_entry(modname, NULL);
    printk("%s, removing..\n",modname);
}

//module_init 函数是必须调用的
module_init(initialization_function)
module_exit(deinitialization_function)

MODULE_AUTHOR("Lyle");
MODULE_DESCRIPTION("user /proc to output data");
MODULE_LICENSE("GPL");
