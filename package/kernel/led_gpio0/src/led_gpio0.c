#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
//#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/uaccess.h>

static struct class  *firstdrv_class;
static struct device *firstdrv_class_dev;
volatile unsigned long *gp0con = NULL;
volatile unsigned long *gp0dat = NULL;

static int first_drv_open(struct inode *inode, struct file *file)
{
    printk("in first_drv_open\n");
    *gp0con |= (1<<0);
    return 0;
}

static ssize_t first_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
    int val;
    printk("in first_drv_write\n");
    copy_from_user(&val, buf, count); 
    if (val == 1)
    {
        // 点灯
        *gp0dat &= ~(1<<0);
		printk("led on success!\n");
    }
    else
    {
        // 灭灯
		*gp0dat &= ~(1<<0);
        *gp0dat |=  (1<<0);
		printk("led off success!\n");
    }
    
    return 0;
}

static struct file_operations first_drv_fops = {
    .owner  =   THIS_MODULE, 
    .open   =   first_drv_open,     
    .write  =   first_drv_write,       
};

int major;

static int first_drv_init(void)
{
    major = register_chrdev(0, "first_drv", &first_drv_fops); // 注册, 告诉内核
                        //参数为0是系统随机分配主设备号
    firstdrv_class = class_create(THIS_MODULE, "firstdrv");
    firstdrv_class_dev = device_create(firstdrv_class, NULL, MKDEV(major, 0), NULL, "led_gpio0"); /* /dev/led_gpio0 */
    //功能是让系统自动生成设备节点
    gp0con = (volatile unsigned long *)ioremap(0x10000600, 4);
    gp0dat = (volatile unsigned long *)ioremap(0x10000620, 4);
	printk("gp0con addr is 0x%x\n", gp0con);
	printk("gp0dat addr is 0x%x\n", gp0dat);
    return 0;
}

static void first_drv_exit(void)
{
    unregister_chrdev(major, "first_drv"); // 卸载
    device_destroy(firstdrv_class, MKDEV(major, 0));
    class_destroy(firstdrv_class);
    iounmap(gp0con);
	iounmap(gp0dat);
	printk("iounmap\n");
}

module_init(first_drv_init);
module_exit(first_drv_exit);
MODULE_LICENSE("GPL");
