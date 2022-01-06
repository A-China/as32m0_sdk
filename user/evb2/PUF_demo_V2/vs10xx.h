#ifndef VS10XX_H
#define VS10XX_H

/** VS10xx SCI Write Command byte is 0x02 */
#define VS_WRITE_COMMAND 0x02
/** VS10xx SCI Read Command byte is 0x03 */
#define VS_READ_COMMAND  0x03

#define SPI_MODE	0x0   /**< VS10xx register */
#define SPI_STATUS	0x1   /**< VS10xx register */
#define SPI_BASS	0x2   /**< VS10xx register */
#define SPI_CLOCKF	0x3   /**< VS10xx register */
#define SPI_DECODE_TIME	0x4   /**< VS10xx register */
#define SPI_AUDATA	0x5   /**< VS10xx register */
#define SPI_WRAM	0x6   /**< VS10xx register */
#define SPI_WRAMADDR	0x7   /**< VS10xx register */
#define SPI_HDAT0	0x8   /**< VS10xx register */
#define SPI_HDAT1	0x9   /**< VS10xx register */
#define SPI_AIADDR	0xa   /**< VS10xx register */
#define SPI_VOL		0xb   /**< VS10xx register */
#define SPI_AICTRL0	0xc   /**< VS10xx register */
#define SPI_AICTRL1	0xd   /**< VS10xx register */
#define SPI_AICTRL2	0xe   /**< VS10xx register */
#define SPI_AICTRL3	0xf   /**< VS10xx register */

#define SM_DIFF           (1<< 0)
#define SM_LAYER12        (1<< 1) /* VS1063, VS1053, VS1033, VS1011 */
#define SM_RECORD_PATH    (1<< 1) /* VS1103 */
#define SM_RESET          (1<< 2)
#define SM_CANCEL         (1<< 3) /* VS1063, VS1053 */
#define SM_OUTOFWAV       (1<< 3) /* VS1033, VS1003, VS1011 */
#define SM_OUTOFMIDI      (1<< 3) /* VS1103 */
#define SM_EARSPEAKER_LO  (1<< 4) /* VS1053, VS1033 */
#define SM_PDOWN          (1<< 4) /* VS1003, VS1103 */
#define SM_TESTS          (1<< 5)
#define SM_STREAM         (1<< 6) /* VS1053, VS1033, VS1003, VS1011 */
#define SM_ICONF          (1<< 6) /* VS1103 */
#define SM_EARSPEAKER_HI  (1<< 7) /* VS1053, VS1033 */
#define SM_DACT           (1<< 8)
#define SM_SDIORD         (1<< 9)
#define SM_SDISHARE       (1<<10)
#define SM_SDINEW         (1<<11)
#define SM_ENCODE         (1<<12) /* VS1063 */
#define SM_ADPCM          (1<<12) /* VS1053, VS1033, VS1003 */
#define SM_EARSPEAKER1103 (1<<12) /* VS1103 */
#define SM_ADPCM_HP       (1<<13) /* VS1033, VS1003 */
#define SM_LINE1          (1<<14) /* VS1063, VS1053 */
#define SM_LINE_IN        (1<<14) /* VS1033, VS1003, VS1103 */
#define SM_CLK_RANGE      (1<<15) /* VS1063, VS1053, VS1033 */
#define SM_ADPCM_1103     (1<<15) /* VS1103 */


#define	AUDIO_XRESET 			GPIO_Pin_0 << (2*8)
#define	AUDIO_DREQ 				GPIO_Pin_1 << (2*8)
#define	AUDIO_XDCS 				GPIO_Pin_2 << (2*8)
#define	AUDIO_XCS 				GPIO_Pin_3 << (2*8)


void init_VS10xx(void);
void writeRegister(unsigned char addressbyte, unsigned int value);
unsigned int readRegister(unsigned char addressbyte); /** Read the 16-bit value of a VS10xx register */
void writeData(unsigned char *databuf); //write 32 bytes to vs1053
void audio_delay(unsigned int count);
void spi_send_byte(unsigned char data);
void spi_send_8byte(unsigned char *data);
void spi_send_32byte_dma(unsigned char *data);
void spi_send_512byte_dma(unsigned char *data);
unsigned char spi_getdata(void);
void reset(void); //reset for vs10xx
void softReset(unsigned char test); /* Soft Reset of VS10xx (Between songs) */
void setFreq(int freq);
void init_dma_spi(void);
  
#endif
