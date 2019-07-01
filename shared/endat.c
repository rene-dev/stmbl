#include "endat.h"
#include "math.h"

uint32_t endat_tx(endat_cmd_t cmd, uint8_t p1, uint16_t p2, uint8_t* buf, endat_data_t* data){
  uint32_t len = 0;

  buf[0] = flip8(cmd);

  buf[1] = flip8(p1);

  buf[2] = flip16(p2) & 0xff;
  buf[3] = (flip16(p2) >> 8) & 0xff;

  data->current_cmd = cmd;
  data->current_addr = p1;
  data->current_value = p2;

  switch(cmd){
    case ENDAT_READ_POS:
      len = 2 + 6;
    break;

    case ENDAT_SELECT_MEM:
      len = 2 + 6 + 8 + 16;
      data->current_mem = p1;
    break;

    case ENDAT_READ_ADDR:
      len = 2 + 6 + 8 + 16;
    break;

    case ENDAT_WRITE_ADDR:

      len = 2 + 6 + 8 + 16;
    break;

    case ENDAT_RESET:
      len = 2 + 6 + 8 + 16;
    break;

    default:
      len = 0;
  }

  return(len);
}

uint32_t endat_rx(uint8_t* buf, uint32_t max_len, endat_data_t* data){
  union{
    uint64_t data64;
    uint32_t data32[2];
    uint16_t data16[4];
    uint8_t data8[8];
  }df;

  uint32_t len = 0;

  endat_cmd_t cmd = data->current_cmd;
  data->current_cmd = 0;

  uint8_t addr = data->current_addr;
  data->current_addr = 0;

  uint16_t value = data->current_value;
  data->current_value = 0;

  switch(cmd){
    case ENDAT_READ_POS:
      len = 1 + 1 + data->pos_bits + data->mpos_bits + 5;
    break;

    case ENDAT_SELECT_MEM:
      len = 1 + 8 + 16 + 5;
    break;

    case ENDAT_READ_ADDR:
      len = 1 + 8 + 16 + 5;
    break;

    case ENDAT_WRITE_ADDR:
      len = 1 + 8 + 16 + 5;
    break;

    case ENDAT_RESET:
      len = 1 + 8 + 16 + 5;
    break;

    default:
      return(0);
  }

  if(max_len < len){ // too short
    return(0);
  }

  for(int i = 0; i < (len + 7) / 8; i++){ // local copy
    df.data8[i] = buf[i];
  }

  uint8_t error_bit = 0;
  uint64_t temp1 = 0;
  uint64_t temp2 = 0;
  uint8_t p1;
  uint16_t p2;

  switch(cmd){
    case ENDAT_READ_POS:
      error_bit = df.data8[0] & 0x10;

      df.data64 >>= 2; // remove start + error bit
      temp1 = df.data64 & bitmask[data->pos_bits];
      df.data64 >>= data->pos_bits; // remove pos
      temp2 = df.data64 & bitmask[data->mpos_bits];
      df.data64 >>= data->mpos_bits; // remove mutliturn pos
      data->crc = df.data64 & bitmask[5];

      //check crc
      data->error_bit = 0;
      if(error_bit){
        data->error_bit = 1;
        //return(0);
      }
      data->pos = temp1;
      data->mpos = temp2;
    break;

    case ENDAT_SELECT_MEM:
      p1 = (df.data16[0] >> 1) & 0xff;
      data->crc = (df.data8[3] >> 1) & 0b11111;
      p1 = flip8(p1);

      //check crc
      if(data->current_mem != p1){
        data->current_mem = 0;
        return(0);
      }
    break;

    case ENDAT_READ_ADDR:
      p1 = (df.data16[0] >> 1) & 0xff;
      p2 = (df.data32[0] >> (1 + 8)) & 0xffff;
      data->crc = (df.data8[3] >> 1) & 0b11111;
      p1 = flip8(p1);
      p2 = flip16(p2);

      //check crc
      if(addr != p1){
        return(0);
      }

      switch(data->current_mem){
        case ENDAT_MEM_STATE:
          switch(p1){
            case 0: // error register
              // data->error = *((endat_state_error_t*) &p2);
              data->error.reg = p2;
            break;

            case 1: // warning register
              // data->warning = *((endat_state_warning_t*) &p2);
              data->warning.reg = p2;
            break;

            default:
              return(0);
          }
        break;

        case ENDAT_MEM_PARAM0:
          switch(p1){
            case ENDAT_ADDR_POS_LEN:
              data->pos_len = p2;
              data->pos_bits = data->pos_len - data->mpos_bits;
            break;

            case ENDAT_ADDR_TYPE:
              data->fb_type = p2;
            break;

            default:
              return(0);
          }
        break;

        case ENDAT_MEM_PARAM1:
          switch(p1){
            case ENDAT_ADDR_MULTITURN:
              data->mpos_bits = log2f(p2) + 0.99;
              data->pos_bits = data->pos_len - data->mpos_bits;
            break;

            case ENDAT_ADDR_RES_LOW:
              data->pos_res = (data->pos_res & 0xffff0000) | p2;
            break;

            case ENDAT_ADDR_RES_HIGH:
              data->pos_res = (data->pos_res & 0xffff) | (p2 << 16);
            break;

            default:
              return(0);
          }
        break;

        case ENDAT_MEM_PARAM2:
          switch(p1){
            case ENDAT_ADDR_MAX_VEL:
              data->max_vel = p2;
            break;

            default:
              return(0);
          }
        break;

        default:
          return(0);
      }

    break;

    case ENDAT_WRITE_ADDR:
      p1 = (df.data16[0] >> 1) & 0xff;
      p2 = (df.data32[0] >> (1 + 8)) & 0xffff;
      data->crc = (df.data8[3] >> 1) & 0b11111;
      p1 = flip8(p1);
      p2 = flip16(p2);

      //check crc
      if(addr != p1){
        return(0);
      }
      if(value != p2){
        return(0);
      }
    break;

    case ENDAT_RESET:
      data->crc = (df.data8[3] >> 1) & 0b11111;

      //check crc
    break;
  }

  return(1);
}

const uint64_t bitmask[] = {
  0b0,
  0b1,
  0b11,
  0b111,
  0b1111,
  0b11111,
  0b111111,
  0b1111111,
  0b11111111,
  0b111111111,
  0b1111111111,
  0b11111111111,
  0b111111111111,
  0b1111111111111,
  0b11111111111111,
  0b111111111111111,
  0b1111111111111111,
  0b11111111111111111,
  0b111111111111111111,
  0b1111111111111111111,
  0b11111111111111111111,
  0b111111111111111111111,
  0b1111111111111111111111,
  0b11111111111111111111111,
  0b111111111111111111111111,
  0b1111111111111111111111111,
  0b11111111111111111111111111,
  0b111111111111111111111111111,
  0b1111111111111111111111111111,
  0b11111111111111111111111111111,
  0b111111111111111111111111111111,
  0b1111111111111111111111111111111,
  0b11111111111111111111111111111111,
  0b111111111111111111111111111111111,
  0b1111111111111111111111111111111111,
  0b11111111111111111111111111111111111,
  0b111111111111111111111111111111111111,
  0b1111111111111111111111111111111111111,
  0b11111111111111111111111111111111111111,
  0b111111111111111111111111111111111111111,
  0b1111111111111111111111111111111111111111,
  0b11111111111111111111111111111111111111111,
  0b111111111111111111111111111111111111111111,
  0b1111111111111111111111111111111111111111111,
  0b11111111111111111111111111111111111111111111,
  0b111111111111111111111111111111111111111111111,
  0b1111111111111111111111111111111111111111111111,
  0b11111111111111111111111111111111111111111111111,
  0b111111111111111111111111111111111111111111111111,  
};