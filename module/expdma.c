#include <linux/module.h>
#include <linux/uaccess.h>

#include "expdma.h"
#include "expmain.h"

unsigned long DMABufPtr;
//dma_addr_t    DMABufAddr; 

//ownership of buffer controlled by the BufFlag
// 0: cpu owns buf 0 and device owns buf 1 
// 1: cpu owns buf 1 and device owns buf 0
unsigned char RxBufFlag;
unsigned char TxBufFlag;

unsigned char RxDone;
unsigned char TxDone;

unsigned int cur_rd_pos, cur_wr_pos;

unsigned int BUF_ORDER = 0;
unsigned int BUF_SIZE;

//extern struct pci_dev *PciDev;
unsigned int CONTROL;

int allocate_DMABuffer(struct astra *card, unsigned int order)
{
	BUF_ORDER = order;
	BUF_SIZE = PAGE_SIZE * (1 << BUF_ORDER);
	DMABufPtr = __get_free_pages(__GFP_DMA, BUF_ORDER);
	card->dmabufaddr = pci_map_single(card->pci, (void*)DMABufPtr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);
	pci_dma_sync_single_for_device(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);
	printk("DMABufAddr = %x\n", (unsigned int)card->dmabufaddr);
	return 0;
}

void free_DMABuffer(struct astra *card)
{
	pci_unmap_single(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);
	free_pages(DMABufPtr, BUF_ORDER);
}

int dma_init(void)
{
	BUF_SIZE = PAGE_SIZE * (1 << BUF_ORDER);
	TxBufFlag = 0;
	RxBufFlag = 0;
	TxDone = 1;
	RxDone = 0;
	cur_rd_pos = BUF_SIZE;
	cur_wr_pos = 0;
	return 0;
}

void dma_exit(struct astra *card)
{
	free_DMABuffer(card);
}

unsigned int dma_buf_write(struct astra *card, const char __user *buf, unsigned int count, unsigned int pos)
{
	unsigned long len_attempt, len_succeed;
	unsigned long space_avail;
	void *to;
	void *from;

/*	printk("dma_buf_write: DMABufAddr = %x\n", (unsigned int)card->dmabufaddr); */
	space_avail = BUF_SIZE - pos;
	len_attempt = (space_avail < count) ? space_avail : count;
	to = (void*)(DMABufPtr + pos);
	from = (void*)buf;
	pci_dma_sync_single_for_cpu(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);
	len_succeed = len_attempt - copy_from_user(to, from, len_attempt);
	pci_dma_sync_single_for_device(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);

	return len_succeed;
}

unsigned int dma_buf_read(struct astra *card, char __user *buf, unsigned int count, unsigned int pos)
{
	unsigned int len_attempt, len_succeed;
	unsigned data_avail;
	void *to;
	void *from;

/*	printk("dma_buf_read: DMABufAddr = %x\n", (unsigned int)card->dmabufaddr); */
	data_avail = BUF_SIZE - pos;
	len_attempt = (data_avail < count) ? data_avail : count; 
	to = (void*)buf;
	from = (void*)(DMABufPtr + pos);
	pci_dma_sync_single_for_cpu(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);  
	len_succeed = len_attempt - copy_to_user(to, from, len_attempt);
	pci_dma_sync_single_for_device(card->pci, card->dmabufaddr, BUF_SIZE, PCI_DMA_BIDIRECTIONAL);

	return len_succeed;
}

