//--------------------------------------------------------------
// File     : usbd_cdc_vcp.c
//--------------------------------------------------------------



//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usbd_cdc_vcp.h"


LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };


//--------------------------------------------------------------
extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

//--------------------------------------------------------------
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

uint8_t APP_Tx_Buffer[APP_TX_BUF_SIZE];
uint32_t APP_tx_ptr_head;
uint32_t APP_tx_ptr_tail;
uint8_t APP_tx_end_cmd;

CDC_IF_Prop_TypeDef VCP_fops = 
{
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};


//--------------------------------------------------------------
static uint16_t VCP_Init(void)
{
  APP_tx_ptr_head=0;
  APP_tx_ptr_tail=0;
  APP_tx_end_cmd=0;
  return USBD_OK;
}

//--------------------------------------------------------------
static uint16_t VCP_DeInit(void)
{
  return USBD_OK;
}


//--------------------------------------------------------------
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    /* Not  needed for this driver */
    break;

  case GET_ENCAPSULATED_RESPONSE:
    /* Not  needed for this driver */
    break;

  case SET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case GET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case CLEAR_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case SET_LINE_CODING:
    linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
    linecoding.format = Buf[4];
    linecoding.paritytype = Buf[5];
    linecoding.datatype = Buf[6];
    break;

  case GET_LINE_CODING:
    Buf[0] = (uint8_t)(linecoding.bitrate);
    Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
    Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
    Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
    Buf[4] = linecoding.format;
    Buf[5] = linecoding.paritytype;
    Buf[6] = linecoding.datatype; 
    break;

  case SET_CONTROL_LINE_STATE:
    /* Not  needed for this driver */
    break;

  case SEND_BREAK:
    /* Not  needed for this driver */
    break;    
    
  default:
    break;
  }

  return USBD_OK;
}


//--------------------------------------------------------------
static uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len)
{
  uint32_t i=0;

  while(i < Len) {
    APP_Rx_Buffer[APP_Rx_ptr_in] = *(Buf + i);
    APP_Rx_ptr_in++;
    i++;
    /* To avoid buffer overflow */
    if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE)
    {
      APP_Rx_ptr_in = 0;
    }
  }

  return USBD_OK;
}


//--------------------------------------------------------------
// wird beim empfang von einem Zeichen aufgerufen
//--------------------------------------------------------------
static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len){
  uint32_t i;
  uint32_t temphead;
  uint8_t wert;

  for (i = 0; i < Len; i++){
    temphead=(APP_tx_ptr_head+1) & APP_TX_BUF_MASK;
    APP_tx_ptr_head=temphead;

    if(temphead==APP_tx_ptr_tail) {
      return USBD_FAIL; // overflow
    }

    wert=*(Buf + i);
    if(wert==USB_CDC_RX_END_CHR) {
      // Endekennung wurde empfangen
      APP_tx_end_cmd++;
    }
    APP_Tx_Buffer[temphead] = wert;
  }
  return USBD_OK;
}


//--------------------------------------------------------------
// Ein Byte in den Sendepuffer eintragen
//--------------------------------------------------------------
void UB_VCP_DataTx (uint8_t wert)
{

  APP_Rx_Buffer[APP_Rx_ptr_in] = wert;
  APP_Rx_ptr_in++;

  if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE)
  {
    APP_Rx_ptr_in = 0;
  }
}


//--------------------------------------------------------------
// Einen String aus dem Empfangspuffer auslesen
// bis zur ersten Endekennung (0x0D)
// Ret_wert :
//   0 = nichts vorhanden
//  >0 = Anzahl der empfangenen Zeichen
//--------------------------------------------------------------
uint16_t UB_VCP_StringRx(char *ptr)
{
  uint16_t akt_pos=0;
  uint8_t wert;
  uint32_t temptail;

  // test ob eine Endekennung empfangen wurde
  if(APP_tx_end_cmd==0) return(0);

  if(APP_tx_ptr_head==APP_tx_ptr_tail) {
    // Puffer ist leer
    APP_tx_end_cmd=0;
    return(0);
  }

  // kompletten String bis zur Endekennung auslesen
  // (oder bis Puffer leer ist)
  // es werden nur Ascii-Zeichen übergeben
  akt_pos=0;
  do {
    temptail=(APP_tx_ptr_tail+1) & APP_TX_BUF_MASK;
    APP_tx_ptr_tail=temptail;
    wert=APP_Tx_Buffer[temptail];
    if((wert>=USB_CDC_FIRST_ASCII) && (wert<=USB_CDC_LAST_ASCII)) {
      *(ptr+akt_pos)=wert;
      akt_pos++;
    }
  }while((APP_tx_ptr_head!=APP_tx_ptr_tail) && (wert!=USB_CDC_RX_END_CHR));

  // Stringende anhängen
  *(ptr+akt_pos)=0x00;

  // eine Endekennung wurde bearbeitet
  APP_tx_end_cmd--;

  return akt_pos;
}


