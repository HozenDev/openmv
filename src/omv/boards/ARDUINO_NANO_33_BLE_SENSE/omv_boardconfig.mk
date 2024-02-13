MCU=NRF52840
CPU=cortex-m4
FPU=fpv4-sp-d16
PORT=nrf
SD=s140
HAL_DIR=hal/nrfx
CMSIS_MCU_H='<nrf52840.h>'
NRF_SOFTDEV=s140_nrf52_6.1.1
ARM_MATH=ARM_MATH_CM4
CFLAGS_MCU=NRF52_SERIES
OMV_BOARD_EXTRA_CFLAGS = -DCONFIG_GPIO_AS_PINRESET -DNRF52840_XXAA -DSOFTDEVICE_PRESENT -DBLUETOOTH_SD_DEBUG=1 -DBLUETOOTH_SD=140
VECT_TAB_OFFSET=0x26000
MAIN_APP_ADDR=0x00026000
OMV_HSE_VALUE=12000000
DFU_DEVICE=0x0483:0xdf11
MICROPY_PY_ULAB=1
MICROPY_PY_AUDIO=1
MICROPY_PY_DISPLAY = 0
MICROPY_PY_TV = 0
MICROPY_PY_BUZZER = 0
MICROPY_PY_SENSOR = 1
