/*  
 *   *  hello-1.c - The simplest kernel module.
 *    */
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */

static int debug_enable = 0;
module_param(debug_enable, int, 0);
MODULE_PARM_DESC(debug_enable, "ENABLE DEBUG MODE");

static int __init hello(void)
{
        printk(KERN_EMERG "Hello world 1.\nCurrently debug is: %s\n ",debug_enable ? "enabled":"disabled"); 

            /* 
             *   * A non 0 return means init_module failed; module can't be
             *   loaded. 
             *       */
            return 0;
}

static void __exit cleanup(void)
{
        printk(KERN_INFO "Goodbye world 1.\n");
}

module_init(hello);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OMUJI");
MODULE_DESCRIPTION("SIMPLE HW MOD");
MODULE_VERSION("1.0");
