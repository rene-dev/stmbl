#include "usbd_cdc_if.h"
#include "ringbuf.h"
#define RX_QUEUE_SIZE 1024
#define TX_QUEUE_SIZE 4096

struct ringbuf rx_buf = RINGBUF(RX_QUEUE_SIZE);
struct ringbuf tx_buf = RINGBUF(TX_QUEUE_SIZE);

#define APP_RX_DATA_SIZE 64
#define APP_TX_DATA_SIZE 64
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t *pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
    {
        CDC_Init_FS,
        CDC_DeInit_FS,
        CDC_Control_FS,
        CDC_Receive_FS};

static int8_t CDC_Init_FS(void) {
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

static int8_t CDC_DeInit_FS(void) {
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

static int8_t CDC_Control_FS(uint8_t cmd, uint8_t *pbuf, uint16_t length) {
  /* USER CODE BEGIN 5 */
  switch(cmd) {
    case CDC_SEND_ENCAPSULATED_COMMAND:

      break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

      break;

    case CDC_SET_COMM_FEATURE:

      break;

    case CDC_GET_COMM_FEATURE:

      break;

    case CDC_CLEAR_COMM_FEATURE:

      break;

    /*******************************************************************************/
    /* Line Coding Structure                                                       */
    /*-----------------------------------------------------------------------------*/
    /* Offset | Field       | Size | Value  | Description                          */
    /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
    /* 4      | bCharFormat |   1  | Number | Stop bits                            */
    /*                                        0 - 1 Stop bit                       */
    /*                                        1 - 1.5 Stop bits                    */
    /*                                        2 - 2 Stop bits                      */
    /* 5      | bParityType |  1   | Number | Parity                               */
    /*                                        0 - None                             */
    /*                                        1 - Odd                              */
    /*                                        2 - Even                             */
    /*                                        3 - Mark                             */
    /*                                        4 - Space                            */
    /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
    /*******************************************************************************/
    case CDC_SET_LINE_CODING:

      break;

    case CDC_GET_LINE_CODING:

      break;

    case CDC_SET_CONTROL_LINE_STATE:

      break;

    case CDC_SEND_BREAK:

      break;

    default:
      break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len) {
  /* USER CODE BEGIN 6 */
  rb_write(&rx_buf, Buf, *Len);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len) {
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
  if(hcdc->TxState != 0) {
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void cdc_usbtx() {
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
  if(hcdc->TxState != 0) {
    return;  //busy
  }
  int len = rb_read(&tx_buf, UserTxBufferFS, sizeof(UserTxBufferFS));
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, len);
  USBD_CDC_TransmitPacket(&hUsbDeviceFS);
}

void cdc_poll() {
  if(cdc_is_connected()) {
    cdc_usbtx();
  }
}

int cdc_is_connected() {
  if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) {
    return 0;
  } else {
    return 1;
  }
}

int cdc_tx(void *data, uint32_t len) {
  if(cdc_is_connected()) {
      int ret;
      ret = rb_write(&tx_buf, data, len);
      cdc_usbtx();
      return ret;
    } else {
      return 0;
    }
  }

int cdc_getline(char *ptr, int len) {
  return rb_getline(&rx_buf, ptr, len);
}

int _write(int file, char *ptr, int len) {
  return cdc_tx(ptr, len);
}
