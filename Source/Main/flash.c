#include "A31G21x.h"
#include "A31G21x_hal_aa_types.h"
#include "flash.h"

#define	FMCFG_ENTRY				0x78580300
#define	FMCFG_EXIT				0x78580300

#define	BV_LOCKSEL			(1<<24)
#define	BV_SELFPGM			(1<<23)
#define	BV_TIMEREN			(1<<20)
#define	BV_CFGRDEN			(1<<15)
#define	BV_IFEN					(1<<12)
#define	BV_BBLOCK				(1<<8)
//#define	BV_AE						(1<<7)
#define	BV_MAS					(1<<7)
#define	BV_4KEN					(1<<6)
#define	BV_1KEN					(1<<5)
#define	BV_PMOD				(1<<4)
#define	BV_WADCK				(1<<3)
#define	BV_PGM					(1<<2)
#define	BV_ERS					(1<<1)
#define	BV_HVEN					(1<<0)

#define	FMMR_BUSY				(FMC->BUSY&0x01)	//__bita(FMBUSY,0)

#define	FMCR_LOCKSEL_SET			(FMC->CR|= BV_LOCKSEL)	
#define	FMCR_LOCKSEL_CLR			(FMC->CR&=~BV_LOCKSEL)
#define	FMCR_SELFPGM_SET			(FMC->CR|= BV_SELFPGM)
#define	FMCR_SELFPGM_CLR			(FMC->CR&=~BV_SELFPGM)
#define	FMCR_TIMEREN_SET			(FMC->CR|= BV_TIMEREN)
#define	FMCR_TIMEREN_CLR			(FMC->CR&=~BV_TIMEREN)
#define	FMCR_CFGRDEN_SET			(FMC->CR|= BV_CFGRDEN)
#define	FMCR_CFGRDEN_CLR			(FMC->CR&=~BV_CFGRDEN)
#define	FMCR_IFEN_SET			(FMC->CR|= BV_CFGRDEN)		//__bita(FMCR,12)
#define	FMCR_IFEN_CLR			(FMC->CR&=~BV_CFGRDEN)		//__bita(FMCR,12)
#define	FMCR_BBLOCK_SET			(FMC->CR|= BV_BBLOCK)			//__bita(FMCR,7)
#define	FMCR_BBLOCK_CLR			(FMC->CR&=~BV_BBLOCK)			//__bita(FMCR,7)
#define	FMCR_MAS_SET				(FMC->CR|= BV_MAS)			//__bita(FMCR,7)
#define	FMCR_MAS_CLR				(FMC->CR&=~BV_MAS)			//__bita(FMCR,7)
#define	FMCR_4KEN_SET			(FMC->CR|= BV_4KEN)		//__bita(FMCR,6)
#define	FMCR_4KEN_CLR			(FMC->CR&=~BV_4KEN)		//__bita(FMCR,6)
#define	FMCR_1KEN_SET			(FMC->CR|= BV_1KEN)		//__bita(FMCR,5)
#define	FMCR_1KEN_CLR			(FMC->CR&=~BV_1KEN)		//__bita(FMCR,5)
#define	FMCR_PMOD_SET			(FMC->CR|= BV_PMOD)		//__bita(FMCR,4)
#define	FMCR_PMOD_CLR			(FMC->CR&=~BV_PMOD)		//__bita(FMCR,4)
#define	FMCR_WADCK_SET			(FMC->CR|= BV_WADCK)		//__bita(FMCR,3)
#define	FMCR_WADCK_CLR			(FMC->CR&=~BV_WADCK)		//__bita(FMCR,3)
#define	FMCR_PGM_SET			(FMC->CR|= BV_PGM)			//__bita(FMCR,2)
#define	FMCR_PGM_CLR			(FMC->CR&=~BV_PGM)			//__bita(FMCR,2)
#define	FMCR_ERS_SET			(FMC->CR|= BV_ERS)			//__bita(FMCR,1)
#define	FMCR_ERS_CLR			(FMC->CR&=~BV_ERS)			//__bita(FMCR,1)
#define	FMCR_HVEN_SET			(FMC->CR|= BV_HVEN)		//__bita(FMCR,0)
#define	FMCR_HVEN_CLR			(FMC->CR&=~BV_HVEN)		//__bita(FMCR,0)

void	__SET_OTP(unsigned long otp)
{
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;
	if(otp==0) {
		FMC->MR = 0;
	}
	else {
		FMC->MR = (otp<<28);// | BV_OTPE;
		FMCR_IFEN_SET;
		FMCR_4KEN_CLR;
		//FMCR_1KEN_CLR;
		//FMCR_BBLOCK_SET;
	}
}

void	__WAIT(void)
{
	while(1) {
		// wait until MR_BUSYE is "0"
		if (FMMR_BUSY==0) 
			break; 
	}
}

void	__FLASH_ALL_ENABLE(void)
{
	FMC->MR = 0x66;
	FMC->MR = 0x99;
	FMC->WPROT = 0x00000000;	// Remove FLASH write protection
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

int EraseSector (unsigned long addr)
{
	__FLASH_ALL_ENABLE();
	
	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;
	
	FMC->CR |= (BV_SELFPGM | BV_ERS);
	__NOP();__NOP();__NOP();__NOP();__NOP();
	
	*(unsigned long *)(addr) = 0xFFFFFFFF;
	
	__NOP();__NOP();__NOP();__NOP();
	
	FMC->CR &= ~(BV_ERS);
	FMC->CR = 0x00000000;
	__NOP();__NOP();__NOP();__NOP();__NOP();
	
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return (0); 
}

int ProgramPage (unsigned long addr, unsigned long size, unsigned long *buf)
{
	int	i;
	unsigned long *ptr;
	
	if((addr & 0x03) || (size & 0x03)) {
		return (1); 
	}

	ptr = (unsigned long*)buf;

	__FLASH_ALL_ENABLE();
	
	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;	

	// flash mode entry 
	FMC->MR = 0x5A;FMC->MR = 0xA5;

	FMC->CR |= (BV_SELFPGM | BV_PGM);
	__NOP();__NOP();__NOP();__NOP();__NOP();
	
	for(i=0;i<size;i+=4) {
		*(unsigned long *)(addr) =  *ptr;		
		addr+=4;
		ptr++;
	}
	__NOP();__NOP();__NOP();__NOP();
	
	FMC->CR &= ~(BV_PGM);
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x81;FMC->MR = 0x28;	
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	return 0;
}


unsigned long Verify (unsigned long addr, unsigned long size, unsigned char *buf)
{
	unsigned long	i;
	unsigned long	*src;
	unsigned long	*dst;
	unsigned long	result; 

	if((addr & 0x03) || (size & 0x03)) {
		return (1); 
	}

	result = addr + size; 

	FMC->MR = 0x81;FMC->MR = 0x28;
	FMC->CFG= FMCFG_ENTRY;
	FMC->MR = 0x00;FMC->MR = 0x00;
	
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	FMC->CR = 0x00000000;
	
	FMC->MR = 0x81;FMC->MR = 0x28;		
	FMC->CFG = FMCFG_EXIT; 
	FMC->MR = 0x00;FMC->MR = 0x00;

	return (result);
}
