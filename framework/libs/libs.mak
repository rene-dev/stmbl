include ../../framework/libs/*/lib.mak
#TODO move

INCDIRS += build/inc/
INCDIRS += build/inc/shared_comps/

CPPFLAGS += -DUSBD_PRODUCT_STRING='"STMBL Virtual ComPort"'
CPPFLAGS += -DCDC_IN_FRAME_INTERVAL=1
CPPFLAGS += -DAPP_RX_DATA_SIZE=4096