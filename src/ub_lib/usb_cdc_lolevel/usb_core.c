//--------------------------------------------------------------
// File     : usb_core.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_core.h"
#include "usb_bsp.h"








//--------------------------------------------------------------
static void USB_OTG_EnableCommonInt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_GINTMSK_TypeDef  int_mask;
  
  int_mask.d32 = 0;
  /* Clear any pending USB_OTG Interrupts */

  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GOTGINT, 0xFFFFFFFF);

  /* Clear any pending interrupts */
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, 0xFFFFFFFF);
  /* Enable the interrupts in the INTMSK */
  int_mask.b.wkupintr = 1;
  int_mask.b.usbsuspend = 1; 
  

  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32);
}

//--------------------------------------------------------------
static USB_OTG_STS USB_OTG_CoreReset(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  uint32_t count = 0;
  
  greset.d32 = 0;
  /* Wait for AHB master IDLE state. */
  do
  {
    USB_OTG_BSP_uDelay(3);
    greset.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      return USB_OTG_OK;
    }
  }
  while (greset.b.ahbidle == 0);
  /* Core Soft Reset */
  count = 0;
  greset.b.csftrst = 1;
  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.csftrst == 1);
  /* Wait for 3 PHY Clocks*/
  USB_OTG_BSP_uDelay(3);
  return status;
}

//--------------------------------------------------------------
USB_OTG_STS USB_OTG_WritePacket(USB_OTG_CORE_HANDLE *pdev, 
                                uint8_t             *src, 
                                uint8_t             ch_ep_num, 
                                uint16_t            len)
{
  USB_OTG_STS status = USB_OTG_OK;
  if (pdev->cfg.dma_enable == 0)
  {
    uint32_t count32b= 0 , i= 0;
    __IO uint32_t *fifo;
    
    count32b =  (len + 3) / 4;
    fifo = pdev->regs.DFIFO[ch_ep_num];
    for (i = 0; i < count32b; i++, src+=4)
    {
      USB_OTG_WRITE_REG32( fifo, *((__packed uint32_t *)src) );
    }
  }
  return status;
}


//--------------------------------------------------------------
void *USB_OTG_ReadPacket(USB_OTG_CORE_HANDLE *pdev, 
                         uint8_t *dest, 
                         uint16_t len)
{
  uint32_t i=0;
  uint32_t count32b = (len + 3) / 4;
  
  __IO uint32_t *fifo = pdev->regs.DFIFO[0];
  
  for ( i = 0; i < count32b; i++, dest += 4 )
  {
    *(__packed uint32_t *)dest = USB_OTG_READ_REG32(fifo);
    
  }
  return ((void *)dest);
}

//--------------------------------------------------------------
USB_OTG_STS USB_OTG_SelectCore(USB_OTG_CORE_HANDLE *pdev, 
                               USB_OTG_CORE_ID_TypeDef coreID)
{
  uint32_t i , baseAddress = 0;
  USB_OTG_STS status = USB_OTG_OK;
  
  pdev->cfg.dma_enable       = 0;
  
  /* at startup the core is in FS mode */
  pdev->cfg.speed            = USB_OTG_SPEED_FULL;
  pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;    
    
  /* initialize device cfg following its address */
  if (coreID == USB_OTG_FS_CORE_ID)
  {
    baseAddress                = USB_OTG_FS_BASE_ADDR;
    pdev->cfg.coreID           = USB_OTG_FS_CORE_ID;
    pdev->cfg.host_channels    = 8 ;
    pdev->cfg.dev_endpoints    = 4 ;
    pdev->cfg.TotalFifoSize    = 320; /* in 32-bits */
    pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;     
    

  }
  else if (coreID == USB_OTG_HS_CORE_ID)
  {
    baseAddress                = USB_OTG_HS_BASE_ADDR;
    pdev->cfg.coreID           = USB_OTG_HS_CORE_ID;    
    pdev->cfg.host_channels    = 12 ;
    pdev->cfg.dev_endpoints    = 6 ;
    pdev->cfg.TotalFifoSize    = 1280;/* in 32-bits */
    
  }
  
  pdev->regs.GREGS = (USB_OTG_GREGS *)(baseAddress + \
    USB_OTG_CORE_GLOBAL_REGS_OFFSET);
  pdev->regs.DREGS =  (USB_OTG_DREGS  *)  (baseAddress + \
    USB_OTG_DEV_GLOBAL_REG_OFFSET);
  
  for (i = 0; i < pdev->cfg.dev_endpoints; i++)
  {
    pdev->regs.INEP_REGS[i]  = (USB_OTG_INEPREGS *)  \
      (baseAddress + USB_OTG_DEV_IN_EP_REG_OFFSET + \
        (i * USB_OTG_EP_REG_OFFSET));
    pdev->regs.OUTEP_REGS[i] = (USB_OTG_OUTEPREGS *) \
      (baseAddress + USB_OTG_DEV_OUT_EP_REG_OFFSET + \
        (i * USB_OTG_EP_REG_OFFSET));
  }
  pdev->regs.HREGS = (USB_OTG_HREGS *)(baseAddress + \
    USB_OTG_HOST_GLOBAL_REG_OFFSET);
  pdev->regs.HPRT0 = (uint32_t *)(baseAddress + USB_OTG_HOST_PORT_REGS_OFFSET);
  
  for (i = 0; i < pdev->cfg.host_channels; i++)
  {
    pdev->regs.HC_REGS[i] = (USB_OTG_HC_REGS *)(baseAddress + \
      USB_OTG_HOST_CHAN_REGS_OFFSET + \
        (i * USB_OTG_CHAN_REGS_OFFSET));
  }
  for (i = 0; i < pdev->cfg.host_channels; i++)
  {
    pdev->regs.DFIFO[i] = (uint32_t *)(baseAddress + USB_OTG_DATA_FIFO_OFFSET +\
      (i * USB_OTG_DATA_FIFO_SIZE));
  }
  pdev->regs.PCGCCTL = (uint32_t *)(baseAddress + USB_OTG_PCGCCTL_OFFSET);
  
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_CoreInit(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_GUSBCFG_TypeDef  usbcfg;
  USB_OTG_GCCFG_TypeDef    gccfg;
  USB_OTG_GI2CCTL_TypeDef  i2cctl;
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;
  
  usbcfg.d32 = 0;
  gccfg.d32 = 0;
  ahbcfg.d32 = 0;
  

  
  if (pdev->cfg.phy_itface == USB_OTG_ULPI_PHY)
  {
    gccfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GCCFG);
    gccfg.b.pwdn = 0;
    
    if (pdev->cfg.Sof_output)
    {
      gccfg.b.sofouten = 1;   
    }
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GCCFG, gccfg.d32);
    
    /* Init The ULPI Interface */
    usbcfg.d32 = 0;
    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
    
    usbcfg.b.physel            = 0; /* HS Interface */

    usbcfg.b.term_sel_dl_pulse = 0; /* Data line pulsing using utmi_txvalid */    
    usbcfg.b.ulpi_utmi_sel     = 1; /* ULPI seleInterfacect */
    
    usbcfg.b.phyif             = 0; /* 8 bits */
    usbcfg.b.ddrsel            = 0; /* single data rate */
    
    usbcfg.b.ulpi_fsls = 0;
    usbcfg.b.ulpi_clk_sus_m = 0;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
    
    /* Reset after a PHY select  */
    USB_OTG_CoreReset(pdev);
    
    if(pdev->cfg.dma_enable == 1)
    {
      
      ahbcfg.b.hburstlen = 5; /* 64 x 32-bits*/
      ahbcfg.b.dmaenable = 1;
      USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32);
      
    }    
  }
  else /* FS interface (embedded Phy or I2C Phy) */
  {
    
    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);;
    usbcfg.b.physel  = 1; /* FS Interface */
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
    /* Reset after a PHY select and set Host mode */
    USB_OTG_CoreReset(pdev);
    /* Enable the I2C interface and deactivate the power down*/
    gccfg.d32 = 0;
    gccfg.b.pwdn = 1;
    
    if(pdev->cfg.phy_itface == USB_OTG_I2C_PHY)
    {
      gccfg.b.i2cifen = 1;
    }   
    gccfg.b.vbussensingA = 1 ;
    gccfg.b.vbussensingB = 1 ;     
    
    if(pdev->cfg.Sof_output)
    {
      gccfg.b.sofouten = 1;  
    }
    
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GCCFG, gccfg.d32);
    USB_OTG_BSP_mDelay(20);
    /* Program GUSBCFG.OtgUtmifsSel to I2C*/
    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
    
    if(pdev->cfg.phy_itface == USB_OTG_I2C_PHY)
    {
      usbcfg.b.otgutmifssel = 1;
    }
    
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
    
    if(pdev->cfg.phy_itface == USB_OTG_I2C_PHY)
    {
      /*Program GI2CCTL.I2CEn*/
      i2cctl.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GI2CCTL);
      i2cctl.b.i2cdevaddr = 1;
      i2cctl.b.i2cen = 0;
      i2cctl.b.dat_se0 = 1;
      i2cctl.b.addr = 0x2D;
      USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GI2CCTL, i2cctl.d32);
      
      USB_OTG_BSP_mDelay(200);
      
      i2cctl.b.i2cen = 1;
      USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GI2CCTL, i2cctl.d32);
      USB_OTG_BSP_mDelay(200);
    }
  }
  /* case the HS core is working in FS mode */
  if(pdev->cfg.dma_enable == 1)
  {
    
    ahbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GAHBCFG);
    ahbcfg.b.hburstlen = 5; /* 64 x 32-bits*/
    ahbcfg.b.dmaenable = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32);
    
  }
  /* initialize OTG features */

  return status;
}
//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EnableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;
  
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GAHBCFG, 0, ahbcfg.d32);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32, 0);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_FlushTxFifo (USB_OTG_CORE_HANDLE *pdev , uint32_t num )
{
  USB_OTG_STS status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  
  uint32_t count = 0;
  greset.d32 = 0;
  greset.b.txfflsh = 1;
  greset.b.txfnum  = num;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.txfflsh == 1);
  /* Wait for 3 PHY Clocks*/
  USB_OTG_BSP_uDelay(3);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_FlushRxFifo( USB_OTG_CORE_HANDLE *pdev )
{
  USB_OTG_STS status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  uint32_t count = 0;
  
  greset.d32 = 0;
  greset.b.rxfflsh = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.rxfflsh == 1);
  /* Wait for 3 PHY Clocks*/
  USB_OTG_BSP_uDelay(3);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_SetCurrentMode(USB_OTG_CORE_HANDLE *pdev , uint8_t mode)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_GUSBCFG_TypeDef  usbcfg;
  
  usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
  
  usbcfg.b.force_host = 0;
  usbcfg.b.force_dev = 0;
  
  if ( mode == HOST_MODE)
  {
    usbcfg.b.force_host = 1;
  }
  else if ( mode == DEVICE_MODE)
  {
    usbcfg.b.force_dev = 1;
  }
  
  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
  USB_OTG_BSP_mDelay(50);
  return status;
}


//--------------------------------------------------------------
uint32_t USB_OTG_GetMode(USB_OTG_CORE_HANDLE *pdev)
{
  return (USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS ) & 0x1);
}


//--------------------------------------------------------------
uint8_t USB_OTG_IsDeviceMode(USB_OTG_CORE_HANDLE *pdev)
{
  return (USB_OTG_GetMode(pdev) != HOST_MODE);
}


//--------------------------------------------------------------
uint8_t USB_OTG_IsHostMode(USB_OTG_CORE_HANDLE *pdev)
{
  return (USB_OTG_GetMode(pdev) == HOST_MODE);
}


//--------------------------------------------------------------
uint32_t USB_OTG_ReadCoreItr(USB_OTG_CORE_HANDLE *pdev)
{
  uint32_t v = 0;
  v = USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS);
  v &= USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTMSK);
  return v;
}


//--------------------------------------------------------------
uint32_t USB_OTG_ReadOtgItr (USB_OTG_CORE_HANDLE *pdev)
{
  return (USB_OTG_READ_REG32 (&pdev->regs.GREGS->GOTGINT));
}



//--------------------------------------------------------------
void USB_OTG_InitDevSpeed(USB_OTG_CORE_HANDLE *pdev , uint8_t speed)
{
  USB_OTG_DCFG_TypeDef   dcfg;
  
  dcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCFG);
  dcfg.b.devspd = speed;
  USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCFG, dcfg.d32);
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_CoreInitDev (USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS             status       = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  uint32_t i;
  USB_OTG_DCFG_TypeDef    dcfg;
  USB_OTG_FSIZ_TypeDef    nptxfifosize;
  USB_OTG_FSIZ_TypeDef    txfifosize;
  USB_OTG_DIEPMSK_TypeDef msk;
  USB_OTG_DTHRCTL_TypeDef dthrctl;  
  
  depctl.d32 = 0;
  dcfg.d32 = 0;
  nptxfifosize.d32 = 0;
  txfifosize.d32 = 0;
  msk.d32 = 0;
  
  /* Restart the Phy Clock */
  USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, 0);
  /* Device configuration register */
  dcfg.d32 = USB_OTG_READ_REG32( &pdev->regs.DREGS->DCFG);
  dcfg.b.perfrint = DCFG_FRAME_INTERVAL_80;
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCFG, dcfg.d32 );
  

  if(pdev->cfg.coreID == USB_OTG_FS_CORE_ID  )
  {  
    
    /* Set Full speed phy */
    USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_FULL);
    
    /* set Rx FIFO size */
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRXFSIZ, RX_FIFO_FS_SIZE);
    
    /* EP0 TX*/
    nptxfifosize.b.depth     = TX0_FIFO_FS_SIZE;
    nptxfifosize.b.startaddr = RX_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF0_HNPTXFSIZ, nptxfifosize.d32 );
    
    
    /* EP1 TX*/
    txfifosize.b.startaddr = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
    txfifosize.b.depth = TX1_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[0], txfifosize.d32 );
    
    
    /* EP2 TX*/
    txfifosize.b.startaddr += txfifosize.b.depth;
    txfifosize.b.depth = TX2_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[1], txfifosize.d32 );
    
    
    /* EP3 TX*/  
    txfifosize.b.startaddr += txfifosize.b.depth;
    txfifosize.b.depth = TX3_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[2], txfifosize.d32 );
  }


  /* Flush the FIFOs */
  USB_OTG_FlushTxFifo(pdev , 0x10); /* all Tx FIFOs */
  USB_OTG_FlushRxFifo(pdev);
  /* Clear all pending Device Interrupts */
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFF );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, 0 );
  
  for (i = 0; i < pdev->cfg.dev_endpoints; i++)
  {
    depctl.d32 = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[i]->DIEPCTL);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPCTL, depctl.d32);
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPTSIZ, 0);
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFF);
  }
  for (i = 0; i <  pdev->cfg.dev_endpoints; i++)
  {
    USB_OTG_DEPCTL_TypeDef  depctl;
    depctl.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[i]->DOEPCTL);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPCTL, depctl.d32);
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPTSIZ, 0);
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
  }
  msk.d32 = 0;
  msk.b.txfifoundrn = 1;
  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPMSK, msk.d32, msk.d32);
  
  if (pdev->cfg.dma_enable == 1)
  {
    dthrctl.d32 = 0;
    dthrctl.b.non_iso_thr_en = 1;
    dthrctl.b.iso_thr_en = 1;
    dthrctl.b.tx_thr_len = 64;
    dthrctl.b.rx_thr_en = 1;
    dthrctl.b.rx_thr_len = 64;
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DTHRCTL, dthrctl.d32);  
  }
  USB_OTG_EnableDevInt(pdev);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EnableDevInt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_GINTMSK_TypeDef  intmsk;
  
  intmsk.d32 = 0;
  
  /* Disable all interrupts. */
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTMSK, 0);
  /* Clear any pending interrupts */
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, 0xFFFFFFFF);
  /* Enable the common interrupts */
  USB_OTG_EnableCommonInt(pdev);
  
  if (pdev->cfg.dma_enable == 0)
  {
    intmsk.b.rxstsqlvl = 1;
  }
  
  /* Enable interrupts matching to the Device mode ONLY */
  intmsk.b.usbsuspend = 1;
  intmsk.b.usbreset   = 1;
  intmsk.b.enumdone   = 1;
  intmsk.b.inepintr   = 1;
  intmsk.b.outepintr  = 1;
  intmsk.b.sofintr    = 1; 

  intmsk.b.incomplisoin    = 1; 
  intmsk.b.incomplisoout    = 1;   

  intmsk.b.sessreqintr    = 1;
  intmsk.b.otgintr    = 1;

  USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);
  return status;
}


//--------------------------------------------------------------
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_DSTS_TypeDef  dsts;
  enum USB_OTG_SPEED speed = USB_SPEED_UNKNOWN;
  
  
  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
  
  switch (dsts.b.enumspd)
  {
  case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
    speed = USB_SPEED_HIGH;
    break;
  case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
  case DSTS_ENUMSPD_FS_PHY_48MHZ:
    speed = USB_SPEED_FULL;
    break;
    
  case DSTS_ENUMSPD_LS_PHY_6MHZ:
    speed = USB_SPEED_LOW;
    break;
  }
  
  return speed;
}


//--------------------------------------------------------------
USB_OTG_STS  USB_OTG_EP0Activate(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_STS             status = USB_OTG_OK;
  USB_OTG_DSTS_TypeDef    dsts;
  USB_OTG_DEPCTL_TypeDef  diepctl;
  USB_OTG_DCTL_TypeDef    dctl;
  
  dctl.d32 = 0;
  /* Read the Device Status and Endpoint 0 Control registers */
  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
  diepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL);
  /* Set the MPS of the IN EP based on the enumeration speed */
  switch (dsts.b.enumspd)
  {
  case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
  case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
  case DSTS_ENUMSPD_FS_PHY_48MHZ:
    diepctl.b.mps = DEP0CTL_MPS_64;
    break;
  case DSTS_ENUMSPD_LS_PHY_6MHZ:
    diepctl.b.mps = DEP0CTL_MPS_8;
    break;
  }
  USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL, diepctl.d32);
  dctl.b.cgnpinnak = 1;
  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, dctl.d32);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EPActivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  USB_OTG_DAINT_TypeDef  daintmsk;
  __IO uint32_t *addr;
  
  
  depctl.d32 = 0;
  daintmsk.d32 = 0;
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
    daintmsk.ep.out = 1 << ep->num;
  }
  /* If the EP is already active don't change the EP Control
  * register. */
  depctl.d32 = USB_OTG_READ_REG32(addr);
  if (!depctl.b.usbactep)
  {
    depctl.b.mps    = ep->maxpacket;
    depctl.b.eptype = ep->type;
    depctl.b.txfnum = ep->tx_fifo_num;
    depctl.b.setd0pid = 1;
    depctl.b.usbactep = 1;
    USB_OTG_WRITE_REG32(addr, depctl.d32);
  }
  /* Enable the Interrupt for this EP */

    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, 0, daintmsk.d32);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  USB_OTG_DAINT_TypeDef  daintmsk;
  __IO uint32_t *addr;
  
  depctl.d32 = 0;
  daintmsk.d32 = 0;  
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
    daintmsk.ep.out = 1 << ep->num;
  }
  depctl.b.usbactep = 0;
  USB_OTG_WRITE_REG32(addr, depctl.d32);
  /* Disable the Interrupt for this EP */
  

    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, daintmsk.d32, 0);
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EPStartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef     depctl;
  USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
  USB_OTG_DSTS_TypeDef       dsts;    
  uint32_t fifoemptymsk = 0;  
  
  depctl.d32 = 0;
  deptsiz.d32 = 0;
  /* IN endpoint */
  if (ep->is_in == 1)
  {
    depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPCTL));
    deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ));
    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      /* Program the transfer size and packet count
      * as follows: xfersize = N * maxpacket +
      * short_packet pktcnt = N + (short_packet
      * exist ? 1 : 0)
      */
      deptsiz.b.xfersize = ep->xfer_len;
      deptsiz.b.pktcnt = (ep->xfer_len - 1 + ep->maxpacket) / ep->maxpacket;

      if (ep->type == EP_TYPE_ISOC)
      {
        deptsiz.b.mc = 1;
      }       
    }
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ, deptsiz.d32);
    
    if (pdev->cfg.dma_enable == 1)
    {
      USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPDMA, ep->dma_addr);
    }
    else
    {
      if (ep->type != EP_TYPE_ISOC)
      {
        /* Enable the Tx FIFO Empty Interrupt for this EP */
        if (ep->xfer_len > 0)
        {
          fifoemptymsk = 1 << ep->num;
          USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0, fifoemptymsk);
        }
      }
    }
    
    
    if (ep->type == EP_TYPE_ISOC)
    {
      dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
      
      if (((dsts.b.soffn)&0x1) == 0)
      {
        depctl.b.setd1pid = 1;
      }
      else
      {
        depctl.b.setd0pid = 1;
      }
    } 
    
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPCTL, depctl.d32);

    if (ep->type == EP_TYPE_ISOC)
    {
      USB_OTG_WritePacket(pdev, ep->xfer_buff, ep->num, ep->xfer_len);   
    }    
  }
  else
  {
    /* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL));
    deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ));
    /* Program the transfer size and packet count as follows:
    * pktcnt = N
    * xfersize = N * maxpacket
    */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      deptsiz.b.pktcnt = (ep->xfer_len + (ep->maxpacket - 1)) / ep->maxpacket;
      deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, deptsiz.d32);
    
    if (pdev->cfg.dma_enable == 1)
    {
      USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPDMA, ep->dma_addr);
    }
    
    if (ep->type == EP_TYPE_ISOC)
    {
      if (ep->even_odd_frame)
      {
        depctl.b.setd1pid = 1;
      }
      else
      {
        depctl.b.setd0pid = 1;
      }
    }
    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL, depctl.d32);
  }
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS                 status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef      depctl;
  USB_OTG_DEP0XFRSIZ_TypeDef  deptsiz;
  USB_OTG_INEPREGS          *in_regs;
  uint32_t fifoemptymsk = 0;
  
  depctl.d32   = 0;
  deptsiz.d32  = 0;
  /* IN endpoint */
  if (ep->is_in == 1)
  {
    in_regs = pdev->regs.INEP_REGS[0];
    depctl.d32  = USB_OTG_READ_REG32(&in_regs->DIEPCTL);
    deptsiz.d32 = USB_OTG_READ_REG32(&in_regs->DIEPTSIZ);
    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;
      
    }
    else
    {
      if (ep->xfer_len > ep->maxpacket)
      {
        ep->xfer_len = ep->maxpacket;
        deptsiz.b.xfersize = ep->maxpacket;
      }
      else
      {
        deptsiz.b.xfersize = ep->xfer_len;
      }
      deptsiz.b.pktcnt = 1;
    }
    USB_OTG_WRITE_REG32(&in_regs->DIEPTSIZ, deptsiz.d32);
    
    if (pdev->cfg.dma_enable == 1)
    {
      USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPDMA, ep->dma_addr);  
    }
    
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&in_regs->DIEPCTL, depctl.d32);
    
    
    
    if (pdev->cfg.dma_enable == 0)
    {
      /* Enable the Tx FIFO Empty Interrupt for this EP */
      if (ep->xfer_len > 0)
      {
        {
          fifoemptymsk |= 1 << ep->num;
          USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0, fifoemptymsk);
        }
      }
    }
  }
  else
  {
    /* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    deptsiz.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ);
    /* Program the transfer size and packet count as follows:
    * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
    * pktcnt = N           */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      ep->xfer_len = ep->maxpacket;
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, deptsiz.d32);
    if (pdev->cfg.dma_enable == 1)
    {
      USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPDMA, ep->dma_addr);
    }
    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32 (&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL), depctl.d32);
    
  }
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EPSetStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO uint32_t *depctl_addr;
  
  depctl.d32 = 0;
  if (ep->is_in == 1)
  {
    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    /* set the disable and stall bits */
    if (depctl.b.epena)
    {
      depctl.b.epdis = 1;
    }
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  else
  {
    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    /* set the stall bit */
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  return status;
}


//--------------------------------------------------------------
USB_OTG_STS USB_OTG_EPClearStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO uint32_t *depctl_addr;
  
  depctl.d32 = 0;
  
  if (ep->is_in == 1)
  {
    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
  }
  else
  {
    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
  }
  depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
  /* clear the stall bits */
  depctl.b.stall = 0;
  if (ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK)
  {
    depctl.b.setd0pid = 1; /* DATA0 */
  }
  USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  return status;
}


//--------------------------------------------------------------
uint32_t USB_OTG_ReadDevAllOutEp_itr(USB_OTG_CORE_HANDLE *pdev)
{
  uint32_t v;
  v  = USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINT);
  v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINTMSK);
  return ((v & 0xffff0000) >> 16);
}


//--------------------------------------------------------------
uint32_t USB_OTG_ReadDevOutEP_itr(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  uint32_t v;
  v  = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT);
  v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DOEPMSK);
  return v;
}


//--------------------------------------------------------------
uint32_t USB_OTG_ReadDevAllInEPItr(USB_OTG_CORE_HANDLE *pdev)
{
  uint32_t v;
  v = USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINT);
  v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINTMSK);
  return (v & 0xffff);
}

//--------------------------------------------------------------
void USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_DEP0XFRSIZ_TypeDef  doeptsize0;
  doeptsize0.d32 = 0;
  doeptsize0.b.supcnt = 3;
  doeptsize0.b.pktcnt = 1;
  doeptsize0.b.xfersize = 8 * 3;
  USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPTSIZ, doeptsize0.d32 );
  
  if (pdev->cfg.dma_enable == 1)
  {
    USB_OTG_DEPCTL_TypeDef  doepctl;
    doepctl.d32 = 0;
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPDMA, 
                        (uint32_t)&pdev->dev.setup_packet);
    
    /* EP enable */
    doepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[0]->DOEPCTL);
    doepctl.b.epena = 1;
    doepctl.d32 = 0x80008000;
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPCTL, doepctl.d32);
  }
}

//--------------------------------------------------------------
void USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev)
{
  
  USB_OTG_DCTL_TypeDef     dctl;
  USB_OTG_DSTS_TypeDef     dsts;
  USB_OTG_PCGCCTL_TypeDef  power;  
  
  if (pdev->dev.DevRemoteWakeup) 
  {
    dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
    if(dsts.b.suspsts == 1)
    {
      if(pdev->cfg.low_power)
      {
        /* un-gate USB Core clock */
        power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0;
        power.b.stoppclk = 0;
        USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
      }   
      /* active Remote wakeup signaling */
      dctl.d32 = 0;
      dctl.b.rmtwkupsig = 1;
      USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, 0, dctl.d32);
      USB_OTG_BSP_mDelay(5);
      USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0 );
    }
  }
}


//--------------------------------------------------------------
void USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pdev)
{
  if(pdev->cfg.low_power)
  {
    
    USB_OTG_DSTS_TypeDef     dsts;
    USB_OTG_PCGCCTL_TypeDef  power; 
    
    dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
    
    if(dsts.b.suspsts == 1)
    {
      /* un-gate USB Core clock */
      power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
      power.b.gatehclk = 0;
      power.b.stoppclk = 0;
      USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
      
    }
  }
}

//--------------------------------------------------------------
void USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pdev)
{
  uint32_t i;
  
  pdev->dev.device_status = 1;
    
  for (i = 0; i < pdev->cfg.dev_endpoints ; i++)
  {
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFF);
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
  }

  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFF );  
  
  /* Flush the FIFO */
  USB_OTG_FlushRxFifo(pdev);
  USB_OTG_FlushTxFifo(pdev ,  0x10 );  
}


//--------------------------------------------------------------
uint32_t USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,USB_OTG_EP *ep)
{
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO uint32_t *depctl_addr;
  uint32_t Status = 0;  
  
  depctl.d32 = 0;
  if (ep->is_in == 1)
  {
    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    
    if (depctl.b.stall == 1)  
      Status = USB_OTG_EP_TX_STALL;
    else if (depctl.b.naksts == 1)
      Status = USB_OTG_EP_TX_NAK;
    else 
      Status = USB_OTG_EP_TX_VALID;     

  }
  else
  {
    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    if (depctl.b.stall == 1)  
      Status = USB_OTG_EP_RX_STALL;
    else if (depctl.b.naksts == 1)
      Status = USB_OTG_EP_RX_NAK;
    else 
      Status = USB_OTG_EP_RX_VALID; 
  } 
  
  /* Return the current status */
  return Status;
}

//--------------------------------------------------------------
void USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep , uint32_t Status)
{
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO uint32_t *depctl_addr;
  
  depctl.d32 = 0;

  /* Process for IN endpoint */
  if (ep->is_in == 1)
  {
    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    
    if (Status == USB_OTG_EP_TX_STALL)  
    {
      USB_OTG_EPSetStall(pdev, ep); return;
    }
    else if (Status == USB_OTG_EP_TX_NAK)
      depctl.b.snak = 1;
    else if (Status == USB_OTG_EP_TX_VALID)
    {
      if (depctl.b.stall == 1)
      {  
        ep->even_odd_frame = 0;
        USB_OTG_EPClearStall(pdev, ep);
        return;
      }      
      depctl.b.cnak = 1;
      depctl.b.usbactep = 1; 
      depctl.b.epena = 1;
    }
    else if (Status == USB_OTG_EP_TX_DIS)
      depctl.b.usbactep = 0;
  } 
  else /* Process for OUT endpoint */
  {
    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);    
    
    if (Status == USB_OTG_EP_RX_STALL)  {
      depctl.b.stall = 1;
    }
    else if (Status == USB_OTG_EP_RX_NAK)
      depctl.b.snak = 1;
    else if (Status == USB_OTG_EP_RX_VALID)
    {
      if (depctl.b.stall == 1)
      {  
        ep->even_odd_frame = 0;
        USB_OTG_EPClearStall(pdev, ep);
        return;
      }  
      depctl.b.cnak = 1;
      depctl.b.usbactep = 1;    
      depctl.b.epena = 1;
    }
    else if (Status == USB_OTG_EP_RX_DIS)
    {
      depctl.b.usbactep = 0;    
    }
  }

  USB_OTG_WRITE_REG32(depctl_addr, depctl.d32); 
}



