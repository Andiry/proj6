#include <linux/cdev.h>

#ifndef REGISTERS_H
#define REGISTERS_H

#define ADDR_MASK 	0xFFFFFFFFFFFFFFF0

/* Controller's Register File area */

#define REG_DCB_HIADDR	0x00
#define REG_DCB_LOADDR	0x04

#define	REG_GO_MASK	0x00000000FFFFFFE0 

#define REG_STATUS 	0x08

#define INTR_PENDING_MASK 0x00000001

#define CUR_DCB_HIADDR	0x0C
#define CUR_DCB_LOADDR	0x10

#define ERR_INJECT	0x100

#define ERR_ENDP1	(1<<22)
#define ERR_TD1		(1<<21)
#define ERR_AT		(3<<19)
#define ERR_CTAG	(1<<18)
#define ERR_TCE		(1<<17)
#define ERR_UDT		(1<<16)
#define ERR_UC		(1<<15)
#define ERR_CE		(7<<12)
#define ERR_LE		(1<<11)
#define ERR_SQE		(1<<10)
#define ERR_LCRCE	(1<<9)
#define ERR_PE		(1<<8)
#define ERR_FMT_FLD	(3<<5)
#define ERR_ET_FLD	0x1f

#define ATS_CTL		0x200

#define ATS_ENABLE	0x00000001

/* End of Controller's Register File area */


/* DMA Command Blocks (DCB) format */

#define DCB_STS		0x00

#define STS_DET		(3<<5) 
#define STS_UR		(1<<4) 
#define STS_RETRY	(1<<3) 
#define STS_MA		(1<<2) 
#define STS_TA		(1<<1) 
#define STS_ERR		(1<<0) 

#define DCB_CMD		0x04

#define CMD_FDE		(1<<7) 
#define CMD_INT		(1<<6) 
#define CMD_SEM		(1<<5) 
#define CMD_TA		(1<<4) 
#define CMD_LOOP	(1<<3) 
#define CMD_INC		(1<<2) 
#define CMD_CONT	(1<<1) 
#define CMD_DIR		(1<<0)

#define DCB_SYSADDR	0x08
#define DCB_INTADDR	0x10

#define DCB_ITER	0x18
#define DCB_SIZE	0x1C

/* End of DMA Command Blocks (DCB) format */


struct astra {
        struct pci_dev *pci;
        struct cdev cdev;

        u64 ctrladdr, memaddr;
        u64 rsrc_start, rsrc_len;
        void __iomem *remap_addr;
        dma_addr_t dmabufaddr;
        int irq;

        int major;
        int minor_start;
        int minor_num;
        int num;
	unsigned int order;

        //spinlock_t reg_lock;
        //struct mutex open_mutex;

        unsigned int msi;
};


#define astra_writel(card, reg, value)\
	writel(value, (card)->remap_addr + reg)
#define astra_readl(card, reg)\
	readl((card)->remap_addr + reg)
#define astra_writew(card, reg, value)\
	writew(value, (card)->remap_addr + reg)
#define astra_readw(card, reg)\
	readw((card)->remap_addr + reg)
#define astra_writeb(card, reg, value)\
	writel(value, (card)->remap_addr + reg)
#define astra_readb(card, reg)\
	readl((card)->remap_addr + reg)

#endif  //REGISTERS_H

