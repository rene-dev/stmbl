#include "usb_cdc.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "usb_dcd_int.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"



#define RX_QUEUE_SIZE  512

struct ringbuf usb_rx_buf = { .buf = (char[RX_QUEUE_SIZE]) {0}, .bufsize = RX_QUEUE_SIZE };


static USB_OTG_CORE_HANDLE  USB_OTG_dev;

static uint16_t VCP_Init(void)
{
    return USBD_OK;
}

static uint16_t VCP_DeInit(void)
{
    return USBD_OK;
}

static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t *Buf, uint32_t Len)
{
    return USBD_OK;
}

// this function is not called
static uint16_t VCP_DataTx(void)
{
    return USBD_OK;
}

uint16_t cr_count = 0;

static uint16_t VCP_DataRx(uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
    	if (buf[i] == 0x0D)
    	    cr_count++;
        rb_putc(&usb_rx_buf, buf[i]);
    }
    return USBD_OK;
}


#define USB_CDC_FIRST_ASCII 32    // erstes Ascii-Zeichen
#define USB_CDC_LAST_ASCII  255   // letztes Ascii-Zeichen
uint16_t USB_VCP_get_string(char *ptr)
{
    uint16_t akt_pos = 0;
    char wert;

    // test ob eine Endekennung empfangen wurde
    if (cr_count == 0) return 0;

    if (usb_rx_buf.len == 0) return 0;

    // kompletten String bis zur Endekennung auslesen
    // (oder bis Puffer leer ist)
    // es werden nur Ascii-Zeichen übergeben
    do {
        rb_getc(&usb_rx_buf, &wert);
        if ((wert >= USB_CDC_FIRST_ASCII) && (wert <= USB_CDC_LAST_ASCII)) {
            *(ptr + akt_pos) = wert;
            akt_pos++;
        }
    } while ((usb_rx_buf.len != 0) && (wert != 0x0D));

    // Stringende anhängen
    *(ptr + akt_pos) = 0x00;

    // eine Endekennung wurde bearbeitet
    cr_count--;

    return akt_pos;
}

void USB_VCP_send_string(unsigned char *ptr)
{
    while (*ptr != 0) {
        // send a queued byte - copy to usb stack buffer
        APP_Rx_Buffer[APP_Rx_ptr_in++] = *ptr;
        ptr++;

            // To avoid buffer overflow
            if (APP_Rx_ptr_in >= APP_RX_DATA_SIZE) {
               APP_Rx_ptr_in = 0;
            }
    }
}

const CDC_IF_Prop_TypeDef VCP_fops = {
    .pIf_Init   = VCP_Init,
    .pIf_DeInit = VCP_DeInit,
    .pIf_Ctrl   = VCP_Ctrl,
    .pIf_DataTx = VCP_DataTx,
    .pIf_DataRx = VCP_DataRx
};

void usb_init(void)
{
    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
}

void OTG_FS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
    // Enable peripheral clocks
    //
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

    // enable I/O compensation cell to reduce the I/O noise on power supply
    SYSCFG->CMPCR = SYSCFG_CMPCR_CMP_PD;

    // Configure DM and DP Pins
    //
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG1_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG1_FS);

    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_AF,
        .GPIO_OType = GPIO_OType_PP,
        .GPIO_PuPd  = GPIO_PuPd_NOPULL
    });
}

void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
    NVIC_Init(&(NVIC_InitTypeDef) {
        .NVIC_IRQChannel = OTG_FS_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 15,
        .NVIC_IRQChannelSubPriority = 0,
        .NVIC_IRQChannelCmd = ENABLE
    });
}


void USB_OTG_BSP_uDelay(const uint32_t usec)
{
    uint32_t count = 0;
    const uint32_t utime = (120 * usec / 7);
    do {
        if (++count > utime) {
            return ;
        }
    }
    while (1);
}


void USB_OTG_BSP_mDelay(const uint32_t msec)
{
    USB_OTG_BSP_uDelay(msec * 1000);   
}

// USB_Usr

enum {
    USB_CDC_DETACHED,
    USB_CDC_CONNECTED
} E_USB_STAT;

uint8_t usb_cdc_status = USB_CDC_DETACHED;

void USBD_USR_Init(void)
{  
    usb_cdc_status = USB_CDC_DETACHED;
}

void USBD_USR_DeviceReset(uint8_t speed)
{
    usb_cdc_status = USB_CDC_DETACHED;
}

void USBD_USR_DeviceConfigured (void)
{
    usb_cdc_status = USB_CDC_CONNECTED;
}

void USBD_USR_DeviceConnected (void)
{
    usb_cdc_status = USB_CDC_DETACHED;
}

void USBD_USR_DeviceDisconnected (void)
{
    usb_cdc_status = USB_CDC_DETACHED;
}

void USBD_USR_DeviceSuspended(void)
{
    usb_cdc_status = USB_CDC_DETACHED;
}

void USBD_USR_DeviceResumed(void)
{
    usb_cdc_status = USB_CDC_DETACHED;
}

USBD_Usr_cb_TypeDef USR_cb =
{
    USBD_USR_Init,
    USBD_USR_DeviceReset,
    USBD_USR_DeviceConfigured,
    USBD_USR_DeviceSuspended,
    USBD_USR_DeviceResumed,
    USBD_USR_DeviceConnected,
    USBD_USR_DeviceDisconnected,
};

uint8_t USB_CDC_is_connected(void)
{
    return usb_cdc_status;
}

