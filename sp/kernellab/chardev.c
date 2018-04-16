/*****************************************************************************
 *
 * M1522.000800
 * SYSTEM PROGRAMMING
 * 
 * Lab2. Kernel Lab
 *
 * chardev.c
 *  - handle reading pmu in kernel space.
 *
*****************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

#include "chardev.h"

#define MSR_VEC_LIMIT 32
#define SUCCESS 0

MODULE_LICENSE("GPL");

static int Device_Open = 0;

static int device_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG
  printk(KERN_ALERT "device_open(%p)\n", file);
#endif
  if (Device_Open)
    return -EBUSY;

  Device_Open++;
  
  try_module_get(THIS_MODULE);
  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
  printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
#endif
  
  Device_Open--;
  
  module_put(THIS_MODULE);
  return SUCCESS;
}

static ssize_t device_read(struct file *file, 
                          char __user *buffer,
                          size_t length,
                          loff_t *offset)
{
  return 0;
}

static ssize_t device_write(struct file *file, 
                          char __user *buffer,
                          size_t length,
                          loff_t *offset)
{
  return 0;
}

static long long read_msr(unsigned int ecx) {
  /* Implement inline assembly for reading msr
   * YOUR CODE HERE */
  unsigned int edx = 0, eax = 0;
  unsigned long long result = 0;
  __asm__ __volatile__("rdmsr" : "=a"(eax), "=d"(edx) : "c"(ecx));
  result = eax | (unsigned long long)edx << 0x20;

  return result;
}

static void write_msr(int ecx, unsigned int eax, unsigned int edx) {
  /* Implement inline assembly for writing msr
   * YOUR CODE HERE */
	__asm__ __volatile__("wrmsr" : : "c"(ecx), "a"(eax), "d"(edx));
}

static long long read_tsc(void)
  /* reference for read_msr & write_msr
   * DON'T MODIFY THIS FUNCTION */
{
  unsigned eax, edx;
  long long result;
  __asm__ __volatile__("rdtsc" : "=a"(eax), "=d"(edx));
  result = eax | (unsigned long long)edx << 0x20;
#ifdef DEBUG
  printk(KERN_ALERT "Module msrdrv: Read 0x%016llx (0x%08x:0x%08x) from TSC\n", result, edx, eax);
#endif
  return result;
}


static long msrdrv_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param)
{
  /* implement ioctl for handling msrdrv commands
   * YOUR CODE HERE */
  struct MsrInOut *msrops;
  int i;
  if (ioctl_num != IOCTL_MSR_CMDS) return 0;
  msrops = (struct MsrInOut*)ioctl_param;
  for (i = 0 ; i <= MSR_VEC_LIMIT ; i++, msrops++) {
	 switch (msrops->op) {
	 case MSR_NOP:
		break;
	 case MSR_STOP:
		return 0;
	 case MSR_READ:
		msrops->value = read_msr(msrops->ecx);
		break;
	 case MSR_WRITE:
		write_msr(msrops->ecx, msrops->eax, msrops->edx);
		break;
	 case MSR_RDTSC:
		msrops->value = read_tsc();
		break;
	 default:
	   return 1;
	 }
  }
  return 0;
}
void mystrcpy(char *dest, const char *src){
  int i;

  for(i=0; src[i]!='\0'; i++) dest[i]=src[i];
  
  return;
}

int get_ptree(unsigned long ioctl_param) {
  struct PtreeInfo* ptree = (struct PtreeInfo*)ioctl_param;
  struct task_struct* tmptask = current;
  int i = 1;

  printk(KERN_INFO "---NOW NEW START get_ptree\n");

  while(tmptask->pid!=0){
	 mystrcpy(ptree->names[i++], tmptask->comm); 
    printk(KERN_INFO "%s\n", ptree->names[i-1]);
	 tmptask=tmptask->parent;
  }
  ptree->names[0][0]=i;

  printk(KERN_INFO "get_ptree ENDing\n");
  return 0;
}

int device_ioctl(struct file *file, 
                unsigned int ioctl_num, 
                unsigned long ioctl_param)
{
  printk(KERN_INFO "device_ioctl, hi!\n");
  switch (ioctl_num) {
    case IOCTL_GET_PTREE:
      return get_ptree(ioctl_param);

    case IOCTL_MSR_CMDS:
	   printk(KERN_INFO "msring\n");
      return msrdrv_ioctl(file, ioctl_num, ioctl_param);

    default:
      printk(KERN_ALERT "not defined ioctl(%d)\n", ioctl_num);
      return -EINVAL;
  }

  return -EINVAL;
}

struct file_operations Fops = {
  .read =           device_read,
  .write =          device_write,
  .unlocked_ioctl = device_ioctl,
  .open =           device_open,
  .release =        device_release,
};

int init_module()
{
  int ret_val;

  ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

  if (ret_val < 0) {
    printk(KERN_ALERT "%s failed with %d\n",
        "Sorry, registering the character device", ret_val);
    return ret_val;
  }

  return SUCCESS;
}

void cleanup_module()
{
  unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}
