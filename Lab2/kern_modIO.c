#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include "modIO_define.h"
static int device_open(struct inode *, struct file*);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops ={
					.read = device_read,
					.write = device_write,
					.open = device_open,
					.release = device_release

};

static int __init kern_modIO_init(void)
{
	printk(KERN_EMERG "Loading modIO\n");
	register_chrdev(251,"THIS MOD", &fops);
	return 0;
}

static void __exit kern_modIO_exit(void)
{
	unregister_chrdev(251,"THIS MOD");
	printk(KERN_ALERT "Unloading modIO\n");
}


char msg_buf[BUF_LEN]; //buffer to receive message
int msg_len = 19;
char msg[20] = "Hello from inside";
int error_no; //for return value of copy_to_user and copy_from_user

static int device_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "modIO is opened\n");
	return 0;
}

static int device_release(struct inode *inode, struct file *file) 
{
	printk(KERN_ALERT "modIO is closed\n");
	return 0;
}

static ssize_t device_read(struct file *filp, char *buf, size_t len, loff_t *off)
{
	//issue by read() from userspace, we need to copy data to user
	printk(KERN_ALERT "reading the device 101\n");
	//we will copy to *buf (which is the pointer to the buffer in the user space), from char * msg, with length msg_len
	error_no = copy_to_user(buf,msg,msg_len);
	printk( KERN_INFO "%d char was put into buffer of Userland : %s\n",msg_len,msg);
	return msg_len;

}


static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off) 
{
	printk(KERN_ALERT "Writing to device\n");
	//copy from the file to msg_buf with size of BUF_LEN
	error_no = copy_from_user(msg_buf,buf,BUF_LEN);
	printk(KERN_INFO "This was put in Motherland: %s\n",msg_buf);
	return 1;
}


module_init(kern_modIO_init);
module_exit(kern_modIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);
MODULE_VERSION(VER);
