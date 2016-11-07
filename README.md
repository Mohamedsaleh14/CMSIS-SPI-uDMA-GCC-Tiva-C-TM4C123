### CMSIS-SPI-uDMA-GCC-Tiva-C-TM4C123

This repository include 
1-CMSIS driver (cmsis)
2-SPI driver (SPID)
3-DMA driver (UDMA)
4-Error handler (ERRH) #TO_BE_DONE
4-Timer driver (TIMD) TO #TO_BE_DONE

##CMSIS
Standard driver created by ARM to handle ARM cortex processor plus module cmsis_NVIC created by mbed-OS to handle interrupt vector table

##SPID
Higly configurable module
To enable certain SPI peripheral,, uncomment its definition and configure the enabled definitions to desired functionality

#void SPID_Init(SPID_SpiPort_T ssix)
Initalizer for SPIn according to the configured parameters in the header file

#void SPID_Enable (SPID_SpiPort_T ssix)
Enable the SPIn peripheral

#void SPID_Disable (SPID_SpiPort_T ssix)
Disable the SPIn peripheral

#SPID_SSI_T SPID_GetEnabledSPI(void)
Getter for the currently enabled SPI peripheral

#uint8_t SPID_SSInSendData(uint16_t data)
Incase UDMA not used, this API used to send data over Tx of the SPI. If transmission handled successfully the function return SW_OK otherwise SW_NOK

#uint16_t SPID_SSInReceiveData(void)
Incase UDMA not used, this API used to receive data from Rx of the SPI.

#SPID_Status_T SPID_GetSSI2Status(void)

##UDMA

