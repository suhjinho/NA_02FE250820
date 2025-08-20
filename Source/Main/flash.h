#ifndef _FLASH_H_
#define	_FLASH_H_

int		flash_erase_macro(int dsel);
int		flash_erase_sector(int dsel, unsigned long addr);
int		flash_write_sector(int dsel, unsigned long addr,unsigned long *pdata);

int EraseSector (unsigned long addr);
int ProgramPage (unsigned long addr, unsigned long size, unsigned long *buf);
unsigned long Verify (unsigned long addr, unsigned long size, unsigned char *buf);

#endif
