#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <stdbool.h> 
#include "rpi_define.h"

//for random int gen purpose
//#include <stdlib.h>

static unsigned int irqNum;
int LEDarr[3] = {RedLED,GreenLED,YellowLED};
static bool ledOn = 0;
static int result; //for init function

static irq_handler_t rpi_gpio_isr(unsigned int irq, void *dev_id, struct pt_regs *regs) 
{
	static int p = 0;
	//we gonna make it look like it works 70% of the time

	int i;
	if (p%5==0) {
	//toggle the LED state
	for(i = 0;i<3;i++) {
		gpio_set_value(LEDarr[i],   (         (int) gpio_get_value(LEDarr[i]) + 1)  %2   );
		}
	}
	else {
		printk(KERN_INFO "HERE IS A LITTLE LESSON IN TRICKERY\n");
	for(i = 0; i< 3;i++) {
		//anyhow set
		gpio_set_value(LEDarr[i],  p%(i+2) == 0 ? 1 : 0);		
		}
	
	}
	
	//set the LED
	p++;
	printk(KERN_ALERT "GPIO Interrupt!\n");
	return (irq_handler_t) IRQ_HANDLED;
}

static void __exit rpi_gpio_exit(void)
{
	gpio_set_value(GreenLED, 0);
	gpio_set_value(YellowLED, 0);
	gpio_set_value(RedLED, 0);
	gpio_free(GreenLED);
	gpio_free(YellowLED);
	gpio_free(RedLED);
	gpio_free(pushBtn);
	free_irq(irqNum, NULL);
	printk(KERN_ALERT "досвида́ния\n");
}

static int __init rpi_gpio_init(void)
{
	printk(KERN_ALERT "Здравствуйте!\nдобро пожаловать в CE3003 LKM лаб 2, практика 3\n");

	

	result = 0;
	ledOn = true;
	gpio_request(GreenLED, "sysfs");
	gpio_direction_output(GreenLED, true); 
	
	gpio_request(GreenLED, "sysfs");
	gpio_direction_output(YellowLED, true); 

	gpio_request(GreenLED, "sysfs");
	gpio_direction_output(RedLED, true); 

	gpio_request(pushBtn, "sysfs");
	gpio_direction_input(pushBtn);
	gpio_set_debounce(pushBtn, 1000);  //set debounce of 1000ms

	irqNum = gpio_to_irq(pushBtn);
	printk(KERN_ALERT "PushBtn mapped to IRQ %d\n", irqNum);
	//map btn to irq, meaning push butn will now trigger an irq
	result = request_irq(irqNum, (irq_handler_t) rpi_gpio_isr, IRQF_TRIGGER_RISING,"rpi_gpio_handler",NULL);
	return result;	
	
}

module_init(rpi_gpio_init);
module_exit(rpi_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);
MODULE_VERSION(VER);




