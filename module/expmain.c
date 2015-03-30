//
// Sample device driver version 0.6 for the Explorer DMA card.
// Add card control I/O block mmap function.
//
//
//

#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#include "expmain.h"
#include "expdma.h"
#include "common.h"


static int enable_msi;
static int astra_major;

module_param(enable_msi, int, 0444);
MODULE_PARM_DESC(enable_msi, "Enable Message Signaled Interrupt (MSI)");
module_param(astra_major, int, 0444);
MODULE_PARM_DESC(astra_major, "Major device number of Astra card");

MODULE_AUTHOR("Sovan Kundu");
MODULE_DESCRIPTION("Simple driver for testing Explorer Hardware");
MODULE_LICENSE("Dual BSD/GPL");


#define ASTRA_VENDOR_ID 0x1022
#define ASTRA_DEVICE_ID 0x8048

#define DRIVER_NAME "expdrv"


int exp_open(struct inode *inode, struct file *filp)
{
	struct astra *card;
	card = container_of(inode->i_cdev, struct astra, cdev);
	filp->private_data = card;
	return 0;
}

int exp_release(struct inode *inode, struct file *filp)
{
	filp->private_data = NULL;
	return 0;
}

ssize_t exp_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	unsigned int len;
	struct astra *card = filp->private_data;
	len = dma_buf_read(card, buf, count, 0);
	return len;
}

ssize_t exp_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	unsigned int len;
	struct astra *card = filp->private_data;
	len = dma_buf_write(card, buf, count, 0);
	return len;
}

loff_t exp_llseek(struct file *filp, loff_t off, int whence)
{
	return off;
}

int exp_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int err = 0;
	unsigned int address, val;

	struct astra *card = filp->private_data;

	if (_IOC_TYPE(cmd) != EXP_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > EXP_IOC_MAXNR) return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));

	if (err) return -EFAULT;

	switch(cmd){
	case EXP_IOCTALLOCDMABUF:
/*		allocate_DMABuffer(card, arg); */
		break;
	case EXP_IOCGDMABUFPA:
		__put_user(card->dmabufaddr, (int __user *)arg);
		break;
	case EXP_IOCTFREEDMABUF:
		free_DMABuffer(card);
		break;
	case EXP_IOCTSTARTDMA:
		val = (unsigned int)(arg >> 32);
		astra_writel(card, REG_DCB_HIADDR, val);
		val = (unsigned int)((arg & REG_GO_MASK) | 0x1);
		astra_writel(card, REG_DCB_LOADDR, val);
		break;
/*	case EXP_IOCTINSERR:
		astra_writel(card, ERR_INJECT, arg);
		break; */
	case EXP_IOCXCFGDATA:
		__get_user(address, (int __user *)arg);
		pci_read_config_dword(card->pci, address, &val);
		__put_user(val, (int __user *)arg);
		break;
	}
	
	return 0;
}

static int exp_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int res;
	u32 vsize = vma->vm_end - vma->vm_start;

	struct astra *card = filp->private_data;

	if (vsize < PAGE_SIZE) {
		printk("astra: mmap size too small. exit.\n");
		return -EINVAL; 

	} 

/*	printk("astra: ctrladdr: 0x%x\n", card->ctrladdr); */
	res = io_remap_pfn_range(vma, vma->vm_start, card->ctrladdr >> PAGE_SHIFT,
               		  PAGE_SIZE, vma->vm_page_prot);
        if(res) {
               	printk("astra: exp_mmap failed\n");
        	return -EAGAIN;
	}
	
	if (vsize > PAGE_SIZE) { 
/*		printk("astra: memaddr: 0x%x\n", card->memaddr); */
		res = io_remap_pfn_range(vma, vma->vm_start + PAGE_SIZE, card->memaddr >> PAGE_SHIFT,
               		  vsize - PAGE_SIZE, vma->vm_page_prot);
	        if(res) {
        	       	printk("astra: exp_mmap failed\n");
        		return -EAGAIN;
		}
        }

	return 0;
}


static struct file_operations exp_fops = {
	.owner = THIS_MODULE,
	.llseek = exp_llseek,
	.read = exp_read,
	.write = exp_write,
	.ioctl = exp_ioctl,
	.open = exp_open,
	.release = exp_release,
	.mmap = exp_mmap,
};

irqreturn_t astra_interrupt(int irq, void *dev_id)
{
	struct astra *card = dev_id;
	unsigned int status, val;
	

	status = astra_readl(card, REG_STATUS);
	if (status & INTR_PENDING_MASK) {
		printk(KERN_ALERT "Interrupt pending flag is 1\n");
    
		val = astra_readl(card, REG_DCB_LOADDR);

		if (val & 0x1)
			printk(KERN_ALERT "Go bit is High\n");
		else
			printk(KERN_ALERT "Go bit is Low\n");
      
		printk(KERN_ALERT "STATUS = 0x%08x before clearing bit\n", status);
		astra_writel(card, REG_STATUS, (status & ~INTR_PENDING_MASK)); 
		status = astra_readl(card, REG_STATUS);
		printk(KERN_ALERT "STATUS = 0x%08x after clearing bit\n", status);    
	}
	else {
		/* printk(KERN_ALERT "Interrupt pending flag is 0\n"); */
		return IRQ_NONE;
	}
	return IRQ_HANDLED;
}

static int astra_request_irq(struct astra *card)
{

#if (LINUX_VERSION_CODE > 0x020617)
	if (request_irq(card->pci->irq, astra_interrupt, IRQF_SHARED | IRQF_DISABLED, DRIVER_NAME, card)) {
		printk (KERN_ERR "astra: unable to acquire irq.\n");
		return -1;
	}
#else
	if (request_irq(card->pci->irq, astra_interrupt, SA_SHIRQ | SA_INTERRUPT, DRIVER_NAME, card)) {
		printk (KERN_ERR "astra: unable to acquire irq.\n");
		return -1;
	}
#endif

	card->irq = card->pci->irq;
	pci_intx(card->pci, !card->msi);
	return 0;
}

static int astra_setup_cdev(struct astra *card, dev_t dev)
{
	int res;

	cdev_init(&card->cdev, &exp_fops);
	card->cdev.owner = THIS_MODULE;
	card->cdev.ops = &exp_fops;

	res = cdev_add(&card->cdev, dev, card->num);
	if(res) {
		unregister_chrdev_region(dev, card->num);
		kfree(card);
		printk(KERN_ALERT "astra: cdev_add failed\n");
		return res;
	}
	
	return 0;
}

static int astra_init(struct astra *card, struct pci_dev *pci)
{
	int res;
	u32 addr_lo, addr_hi;

        printk("astra: enter %s\n", __FUNCTION__);
        res = pci_enable_device(pci);
        if (res < 0)
                return res;

        card->pci = pci;
        card->irq = -1;
        card->msi = enable_msi;
        card->major = astra_major;
        card->minor_start = 0;
        card->num = 1;

	res = pci_request_regions(pci, "explorer");
	if (res < 0) {
		printk(KERN_ALERT "astra: pci_request_regions failed\n");
		goto fail;
	}

	card->order = 4;
	allocate_DMABuffer(card, card->order);

	card->rsrc_start = pci_resource_start(pci, 0);
	card->rsrc_len = pci_resource_len(pci, 0);

	pci_read_config_dword(card->pci, PCI_BASE_ADDRESS_0, &addr_lo);
        pci_read_config_dword(card->pci, PCI_BASE_ADDRESS_1, &addr_hi);

        card->ctrladdr = ((((u64)addr_hi) << 32) | addr_lo) & ADDR_MASK;

	pci_read_config_dword(card->pci, PCI_BASE_ADDRESS_2, &addr_lo);
	pci_read_config_dword(card->pci, PCI_BASE_ADDRESS_3, &addr_hi);

	card->memaddr = ((((u64) addr_hi) << 32) | addr_lo) & ADDR_MASK;

        card->remap_addr = ioremap_nocache(card->rsrc_start, card->rsrc_len);

	return 0;

fail:
	kfree(card);
	pci_disable_device(pci);
	return res;
		
}
 

static int astra_probe(struct pci_dev *pci, const struct pci_device_id *id)
{
	int res;
	struct astra *card;
	dev_t dev;

        printk("astra: enter %s\n", __FUNCTION__);
	card = kzalloc(sizeof(*card), GFP_KERNEL);
        if(!card) {
                printk(KERN_ERR "astra: cannot allocate card\n");
                return -ENOMEM;
        }

	res = astra_init(card, pci);

	if (res < 0)
		return res;

	if (!card->major) {
		res = alloc_chrdev_region(&dev, card->minor_start, card->num, "explorer");
		if(!res) {
			card->major = MAJOR(dev);
			card->minor_start = MINOR(dev);
		}
	} else {
		dev = MKDEV(card->major, card->minor_start);
		res = register_chrdev_region(dev, card->num, "explorer");
	}

	if (res < 0) {
		kfree(card);
		printk(KERN_ERR "astra: allocate cdev region failed\n");
		return res;
	} 

	res = astra_setup_cdev(card, dev);

	if (res < 0)
		return res;

	if (card->msi) {
		if (pci_enable_msi(card->pci) < 0) {
			card->msi = 0;
			printk("astra: using INTx interrupt.\n");
		} else {
			printk("astra: using MSI interrupt.\n");
		}
	} else {
		printk("astra: using INTx interrupt.\n");
	}

	if (astra_request_irq(card) < 0) {
		res = -EBUSY;
		return res;
	}

	dma_init();

	card->pci->current_state = PCI_D0;
	pci_set_drvdata(card->pci, card);

	printk(KERN_ALERT "astra: Initialization completed\n");
	return 0;
}

static void astra_remove(struct pci_dev *pci)
{
	dev_t dev;
	struct astra *card = pci_get_drvdata(pci);	

	dma_exit(card);
	
	pci_disable_device(pci);	
	pci_release_regions(pci);
	free_irq(card->irq, card);

	iounmap(card->remap_addr);

	cdev_del(&card->cdev);
	dev = MKDEV(card->major, card->minor_start);
	unregister_chrdev_region(dev, card->num);
	kfree(card);

	printk(KERN_ALERT "astra: explorer module exit completed\n");
	return;
}

static struct pci_device_id astra_ids[] = {
	{ PCI_DEVICE(ASTRA_VENDOR_ID, ASTRA_DEVICE_ID), },
	{ 0,}
};

MODULE_DEVICE_TABLE(pci, astra_ids);

static struct pci_driver driver = {
	.name = DRIVER_NAME,
	.id_table = astra_ids,
	.probe = astra_probe,
	.remove = astra_remove,
};

static int __init astra_module_init(void)
{
        printk("astra: enter %s\n", __FUNCTION__);
	return pci_register_driver(&driver);
}	

static void __exit astra_module_exit(void)
{
	pci_unregister_driver(&driver);
}

module_init(astra_module_init);
module_exit(astra_module_exit);


