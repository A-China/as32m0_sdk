#include "printf_config.h"
#include "as32m0_uart.h"
#include "as32m0_dma.h"
#include "as32m0_ssp.h"
#include "test_apb_ssp_flash.h"
#include "test_file_system.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_sysctrl.h"
#include "as32m0_adc.h"
#include "test_SD_card.h"
#include "test_puf_SD.h"
#define DEBUG 0
#define SPI_INIT_FREQ_400KHz (OSC_CLK_FREQ/400000)
#define SPI_SD_CARD_FREQ (OSC_CLK_FREQ/8000000)
//#define SIM					0
extern void _sys_exit(void);
static void ssp_config(uint8_t speed);
void init_SPI(uint8_t speed);
void init_dma_spi(void);

void create_file_list(uint8_t *sector_data,struct file_node *file_list, FAT32_info	Fat);
uint8_t File_Check_Sector(uint8_t *data);
// resp_type	
//		0						R1
// 		1						R1b
//		2 					R2
//		3 					R3
//		7 					R7
static uint32_t adc_shot_test(uint8_t channel, uint8_t SAMCTRL, uint8_t clk_div ,uint32_t read_num);
extern struct file_node g_file_list[16];
DMA_sChLLI     pChLLI_1;
DMA_sChCtrl    pChCtrl_1;
DMA_sChConfig  pChConfig_1;
	
DMA_sChLLI*     pChLLI_1_ptr = &pChLLI_1;
DMA_sChCtrl*    pChCtrl_1_ptr = &pChCtrl_1;
DMA_sChConfig*  pChConfig_1_ptr = &pChConfig_1;	

void printf_feature(){
		printf("\n#####################################################################################\n");//
		printf("###																																										\n");//
		printf("#####################################################################################\n");//
	  printf("	Press l to list file(s) \n");
		printf("	Press e to encrypt file(s) (PUF)\n");//
		printf("	Press d to decrypt file(s) (PUF)\n");//
	  printf("	Press s to show puf key \n");
}
int main(void)
{
	uint16_t  i;
	uint8_t data[512]={0x00}, fat[512]={0x00}, in, tmp=0;
	File_Partition_entry P1;
	FAT32_info	F1;
	struct file_node *ptr_file, *head_ptr;
	head_ptr = &g_file_list[0];
	ptr_file = head_ptr;
	ptr_file->nxtfile_node = NULL;
	ptr_file->prevfile_node = NULL;
	// prepare printf
//	APB_PINC->PD0.FuncSel=GIO_FUNC3;
//	APB_PINC->PD1.FuncSel=GIO_FUNC3;
	
//	SYSCTRL_FastClockSwitch_EXT();
//	SYSCTRL_SlowClockSwitch_EXT();	
//	config_uart0(32000000, UART_SETBAUD, UART0_PA45);	
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	printf("==> \n");
	printf("==> \n");
	printf("==> PUF DEMO PROGRAM\n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	init_SPI(SPI_INIT_FREQ_400KHz); // 400KHz
  init_SD_card_PowerUp();
	init_SPI(SPI_SD_CARD_FREQ);  //  8MHz
	init_SD_card_spi_dma();
	SD_spi_read_single_blk(512, 0x00000000, data);
	//SD_printf_datamap(512, 0x00000000, data);

	if(File_Check_Sector(data)){
		printf("Sector Correct!!\n");
	  file_extract_parttition_table(&P1, data);
		SD_spi_read_single_blk(512, P1.LBA_Begin, data);
		//SD_printf_datamap(512, P1.LBA_Begin, data);
		file_extract_FAT32(&F1, data, P1.LBA_Begin);
		printf("Searching for Root directory...\n");
		printf("\\root\n");	
		init_dma_spi();
		printf_feature();
		while(1){
			while(apUART_Check_RXFIFO_EMPTY(APB_UART0) == 1);
			in = SER_GetChar();
			switch(in){
				case 'l':
					ptr_file = head_ptr;
					create_file_list(data, ptr_file, F1);
				  show_file_list(ptr_file);
					printf_feature();
					break;				
				case 'e':
					ptr_file = head_ptr;
					create_file_list(data, ptr_file, F1);
				  ptr_file = encrypt_file_list(ptr_file);
					tmp = return_chosen_file_ptr(ptr_file);
					tmp--;
					for(i=0;i<tmp;i++){
						if(ptr_file != NULL)
							ptr_file = ptr_file->nxtfile_node;
					}
					if(ptr_file == NULL)
						printf("File Not Exist\n");
					else{
						puf_encrypt_decrypt_file(ptr_file, data, fat, F1.cluster_begin_lba, F1.fat_begin_lba, F1.sectors_per_cluster, ENCRYPT_MODE);
						printf("Encrypt Done\n");			
					}
					printf_feature();
					break;
				case 'd':
					ptr_file = head_ptr;
					create_file_list(data, ptr_file, F1);
					ptr_file = puf_file_list(ptr_file);
					tmp = return_chosen_file_ptr(ptr_file);
					tmp--;
					for(i=0;i<tmp;i++){
						if(ptr_file != NULL)
							ptr_file = ptr_file->nxtfile_node;
					}
					if(ptr_file == NULL)
						printf("File Not Exist\n");
					else{
						puf_encrypt_decrypt_file(ptr_file, data, fat, F1.cluster_begin_lba, F1.fat_begin_lba, F1.sectors_per_cluster, DECRYPT_MODE);	
						NVIC_DisableIRQ(n10_TMR0_IRQn);
						printf("Decrypt Done\n");
					}
					printf_feature();
					break;
				case 's':
					show_puf_key();
					printf_feature();
					break;					
				case 27:
					printf_feature();
					break;
				default:
					break;
			}
		}
	}
	else	
		printf("Sector Wrong!!\n");
	while(apUART_Check_BUSY(APB_UART0) == 1);
	printf("#");
  while(1);
}

static void ssp_config(uint8_t speed)
{
	apSSP_sDeviceControlBlock pParam; 
	apSSP_sDeviceControlBlock* pParam_ptr = &pParam;  
	apSSP_Initialize(APB_SPI);
	
	/* Set Device Parameters */
	pParam_ptr->ClockRate        = 0;  		// sspclkout = sspclk/(ClockPrescale*(ClockRate+1))
	pParam_ptr->ClockPrescale    = speed;  	//40=400KHz
	pParam_ptr->eSCLKPhase       = apSSP_SCLKPHASE_TRAILINGEDGE;
	pParam_ptr->eSCLKPolarity    = apSSP_SCLKPOLARITY_IDLELOW;
	pParam_ptr->eFrameFormat     = apSSP_FRAMEFORMAT_MOTOROLASPI;
	pParam_ptr->eDataSize        = apSSP_DATASIZE_8BITS;
	pParam_ptr->eLoopBackMode    = apSSP_LOOPBACKOFF;
	pParam_ptr->eMasterSlaveMode = apSSP_MASTER;
	pParam_ptr->eSlaveOutput     = apSSP_SLAVEOUTPUTDISABLED;
	apSSP_DeviceParametersSet(APB_SPI, pParam_ptr); 
} 

void init_SPI(uint8_t speed){
	PIN_INFO PIN_INFO;
	PIN_INFO.pin_func = GIO_FUNC1;
	PIN_INFO.pin_stat = GIO_PU;
	PIN_INFO.pin_ds		= GIO_DS_2_4;
	PIN_INFO.pin_od   = OD_OFF;
	PIN_INFO.pin_sonof = SONOF_ON;
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 , &PIN_INFO);
	ssp_config(speed);
}

void init_dma_spi(){
	unsigned char i;

	DMA_LittleEndian(AHBMaster_1);	
	// DMA Sync
	for (i=0;i<16;i++)
           DMA_Enable_Sync(i);	
	// Channel 1 Configuration : MEM_TO_PERIPHERAL_DMA_CTRL
	pChLLI_1_ptr->LLIAddr = 0;
	pChLLI_1_ptr->LM      = AHBMaster_1;
	
	pChCtrl_1_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_1_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_1_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_1_ptr->SourInc       = DMA_Bit_Set;
	pChCtrl_1_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->DestBurstSize = DMA_BURST_8;
	pChCtrl_1_ptr->SourBurstSize = DMA_BURST_8;
	pChCtrl_1_ptr->TransferSize  = 32;
	
	pChConfig_1_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig->ActiveChannel 
	pChConfig_1_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_1_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_1_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_1_ptr->FlowCtrl       = DMA_MEM_TO_PERIPHERAL_DMA_CTRL;
	pChConfig_1_ptr->DestPeripheral = 1;  // use
	pChConfig_1_ptr->SourPeripheral = 0;
	pChConfig_1_ptr->ChannelEnable  = DMA_Bit_Set;
}

uint8_t File_Check_Sector(uint8_t *data){
  data = data + 510;
  if((*data == 0x55) && (*++data == 0xAA))
		 return 1;
	else	
		 return 0;
}

void create_file_list(uint8_t *sector_data,struct file_node *file_list, FAT32_info	Fat){
	struct file_node *head_ptr;
	uint32_t i=0, num, cur_addr;
	uint32_t *num_ptr;
	head_ptr = file_list;
	num_ptr = &num;
	*num_ptr = 0;
	cur_addr = Fat.cluster_begin_lba;
		do{
			SD_spi_read_single_blk(512, cur_addr, sector_data);
			file_list = file_extract_directory(cur_addr, sector_data, Fat.cluster_begin_lba, Fat.sectors_per_cluster, file_list, num_ptr);
			cur_addr++;
			i++;
			} while((i<(Fat.sectors_per_cluster)));//For speed
		printf("\n %d File(s)\n", *num_ptr);
		printf("Analyze ...\n");
	  file_list = file_list->prevfile_node;
		file_list->nxtfile_node = NULL;
		file_list = head_ptr;
	#if DEBUG
		i=0;	
		do{
		
			printf("File %d name : %s.%s\n", i, file_list->file.File_name, file_list->file.Exten);
			printf("File %d Name_address : %.8x\n", i, file_list->file.Name_Sector_address);
			printf("File %d Exten index : %.8x\n", i, file_list->file.Exten_index);
			printf("File %d sizes : %d bytes\n", i, file_list->file.File_size);
			printf("File %d First Cluster Address : %.8x\n\n", i, ((file_list->file.First_Cluster_number-2)*Fat.sectors_per_cluster+Fat.cluster_begin_lba));
			printf("File %d First Cluster Number : %.8x\n\n", i, file_list->file.First_Cluster_number);		
			file_list = file_list->nxtfile_node;
			i++;
		}while(file_list != NULL);	
		file_list = head_ptr;	
	#endif

}
