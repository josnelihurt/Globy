#please note that the chip version that we are using is the AA version so please refer to your chip
#if you need to adjust it. AA means RAM=>16K and FLAS=>256K

PROJECT_NAME     := globy_SDK12_3_s130_nrf51822_xxaa
TARGETS          := nrf51822_xxaa
OUTPUT_DIRECTORY := _build
OPENOCD_SCRIPTS_PATH  := C:/Users/josne/AppData/Roaming/xPacks/@xpack-dev-tools/openocd/0.10.0-14.3/.content/scripts/

SDK_ROOT := c:/opt/nRF5_SDK_12
PROJ_DIR := .

$(OUTPUT_DIRECTORY)/nrf51822_xxaa.out: \
  LINKER_SCRIPT  := ble_app_template_gcc_nrf51.ld

# Source files common to all targets
include Makefile.Sources

# Libraries common to all targets
LIB_FILES += \

CFLAGS += -DNRF_LOG_ENABLED
CFLAGS += -DBOARD_PCA10028
#V1
#CFLAGS += -DNRF_LOG_BACKEND_SERIAL_UART_TX_PIN=15 -DNRF_LOG_BACKEND_SERIAL_UART_RX_PIN=11 -DNRF_LOG_LEVEL=3 -DSMARTBAG_USE_ACC=1
#V2
CFLAGS += -DBSP_LED_0=18 -DNRF_LOG_BACKEND_SERIAL_UART_TX_PIN=9 -DNRF_LOG_BACKEND_SERIAL_UART_RX_PIN=11 -DNRF_LOG_LEVEL=3 -DSMARTBAG_USE_ACC=1

#for Quasar Board
#CFLAGS += -DBOARD_QUASAR1 
#CFLAGS += -DNRF_LOG_BACKEND_SERIAL_UART_TX_PIN=9 -DNRF_LOG_LEVEL=3

CFLAGS += -DNRF_LOG_ENABLED
CFLAGS +=  -Wall -Os
#CFLAGS +=  -Wall -O0 -g
#CFLAGS += -DNRF_LOG_ENABLED -DNRF_LOG_BACKEND_SERIAL_UART_TX_PIN=13 -DNRF_LOG_LEVEL=4
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DNRF51
CFLAGS += -DS130
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DNRF51822
CFLAGS += -DNRF_SD_BLE_API_VERSION=2
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs
#CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=soft
# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DBOARD_QUASAR1
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51
ASMFLAGS += -DS130
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DNRF51822
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=2

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

########################

# C flags common to all targets
#CFLAGS += -DSVC_INTERFACE_CALL_AS_NORMAL_FUNCTION 
CFLAGS += -fomit-frame-pointer
#CFLAGS += -flto


# Assembler flags common to all targets
#ASMFLAGS += -DSVC_INTERFACE_CALL_AS_NORMAL_FUNCTION

########################

.PHONY: $(TARGETS) default all clean help flash flash_softdevice

# Default target - first one defined
default: nrf51822_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf51822_xxaa

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

flash: _build/nrf51822_xxaa.hex
	@echo Flashing: $<
	openocd -s $(PROJ_DIR)/scripts -f 'interface/stlink-v2.cfg' -f 'target/nrf51_stlink.tcl' \
	 -c 'init' -c 'reset halt' -c 'nrf51 mass_erase' -c 'sleep 500' \
	 -c 'flash write_image $(SDK_ROOT)/components/softdevice/s130/hex/s130_nrf51_2.0.1_softdevice.hex 0x0'  \
	 -c 'verify_image $(SDK_ROOT)/components/softdevice/s130/hex/s130_nrf51_2.0.1_softdevice.hex 0x0' \
	 -c 'flash write_image $< 0x0'  \
	 -c 'verify_image $< 0x0' \
	 -c 'reset run' \
	 -c 'shutdown'
	