#ifndef EXPDMA_H
#define EXPDMA_H
#include <linux/pci.h>
#include "expmain.h"

int dma_init(void);
void dma_exit(struct astra *card);
unsigned int dma_buf_write(struct astra *card, const char __user *buf, unsigned int count, unsigned int pos);
unsigned int dma_buf_read(struct astra *card, char __user *buf, unsigned int count, unsigned int pos);
int allocate_DMABuffer(struct astra *card, unsigned int order);
void free_DMABuffer(struct astra *card);

#endif //EXPDMA_H

