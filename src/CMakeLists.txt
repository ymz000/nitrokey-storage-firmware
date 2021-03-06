cmake_minimum_required(VERSION 3.9)
project(nitrokey_storage)

set(CMAKE_C_STANDARD 90)

MESSAGE(WARNING "This is only a stub. Not possible to build a firmware with it at the moment.")

# Copied from the AVR32 build log for the further processing
set(NKDEFINES "-DBOARD=EVK1104 -DFREERTOS_USED")
set(NKFLAGS "-O0 -fdata-sections -g3 -Wall -c -fmessage-length=0 -mpart=uc3a3256s -ffunction-sections -masm-addr-pseudos")
set(NKCOMPILER avr32-gcc)
set(NKLINKFLAGS "-nostartfiles -L../src/SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS -L../src/SOFTWARE_FRAMEWORK/BOARDS -Wl,--gc-sections -Wl,-e,_trampoline -T../src/SOFTWARE_FRAMEWORK/UTILS/LINKER_SCRIPTS/AT32UC3A3/256/GCC/link_uc3a3256.lds -mpart=uc3a3256s -Wl,--gc-sections --rodata-writable -Wl,--direct-data -oUSB_MASS.elf -lnewlib_addons-at32ucr2-speed_opt")


include_directories(.)

include_directories(./CCID/LOCAL_ACCESS/)
include_directories(./CCID/UNIT_TEST/)
include_directories(./CCID/USART/)
include_directories(./Cipher/Xts/)
include_directories(./CONFIG/)
include_directories(./FILE_IO/)
include_directories(./HighLevelFunctions/)
include_directories(./INTERNAL_WORK/)
include_directories(./OTP/)
include_directories(./SOFTWARE_FRAMEWORK/BOARDS/)
include_directories(./SOFTWARE_FRAMEWORK/BOARDS/EVK1104/)
include_directories(./SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/DATA_FLASH/AT45DBX/)
include_directories(./SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/ADC/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/AES/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/CPU/CYCLE_COUNTER/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/FLASHC/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/GPIO/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/INTC/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/MCI/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/PM/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/SPI/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/TC/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/USART/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/USBB/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/)
include_directories(./SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/HOST/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/FAT/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/portable/GCC/AVR32_UC3/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/MEMORY/CTRL_ACCESS/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/USB/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/HID/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/HOST_MEM/)
include_directories(./SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/SCSI_DECODER/)
include_directories(./SOFTWARE_FRAMEWORK/UTILS/)
include_directories(./SOFTWARE_FRAMEWORK/UTILS/DEBUG/)
include_directories(./SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/)
include_directories(./SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR/)
include_directories(./Tools/)
include_directories(./USB_CCID/)
include_directories(./USER_INTERFACE/)
include_directories(./USHELL/)
include_directories(./VIRTUAL_MEM/)


add_executable(src
        CCID/LOCAL_ACCESS/Debug_T1.c
        CCID/LOCAL_ACCESS/Debug_T1.h
        CCID/LOCAL_ACCESS/OpenPGP_V20.c
        CCID/LOCAL_ACCESS/OpenPGP_V20.h
        CCID/UNIT_TEST/CONVERT_USB_LOG.c
        CCID/UNIT_TEST/CONVERT_USB_LOG.h
        CCID/UNIT_TEST/SIM_USB_CCID.c
        CCID/UNIT_TEST/SIM_USB_CCID.h
        CCID/USART/ISO7816_ADPU.c
        CCID/USART/ISO7816_ADPU.h
        CCID/USART/ISO7816_Prot_T0.c
        CCID/USART/ISO7816_Prot_T0.h
        CCID/USART/ISO7816_Prot_T1.c
        CCID/USART/ISO7816_Prot_T1.h
        CCID/USART/ISO7816_USART.c
        CCID/USART/ISO7816_USART.h
        CCID/USART/ISO7816_USART_INT.c
        CCID/USART/ISO7816_USART_INT.h
        Cipher/Xts/aes.h
        Cipher/Xts/aescrypt.c
        Cipher/Xts/aeskey.c
        Cipher/Xts/aesopt.h
        Cipher/Xts/aestab.c
        Cipher/Xts/aestab.h
        Cipher/Xts/brg_endian.h
        Cipher/Xts/brg_types.h
        Cipher/Xts/gentest.c
        Cipher/Xts/mode_hdr.h
        Cipher/Xts/modetest.c
        Cipher/Xts/xts.c
        Cipher/Xts/xts.h
        CONFIG/conf_access.h
        CONFIG/conf_at45dbx.h
        CONFIG/conf_explorer.h
        CONFIG/conf_sd_mmc_mci.h
        CONFIG/conf_sd_mmc_spi.h
        CONFIG/conf_usb.h
        CONFIG/FreeRTOSConfig.h
        FILE_IO/FileAccessInterface.c
        FILE_IO/FileAccessInterface.h
        HighLevelFunctions/FlashStorage.c
        HighLevelFunctions/FlashStorage.h
        HighLevelFunctions/HandleAesStorageKey.c
        HighLevelFunctions/HandleAesStorageKey.h
        HighLevelFunctions/HiddenVolume.c
        HighLevelFunctions/HiddenVolume.h
        HighLevelFunctions/MatrixPassword.c
        HighLevelFunctions/MatrixPassword.h
        HighLevelFunctions/password_safe.c
        HighLevelFunctions/password_safe.h
        INTERNAL_WORK/internal_work.c
        INTERNAL_WORK/internal_work.h
        OTP/aes.c
        OTP/aes.h
        OTP/hmac-sha1.c
        OTP/hmac-sha1.h
        OTP/hotp.c
        OTP/hotp.h
        OTP/keyboard.c
        OTP/keyboard.h
        OTP/report_protocol.c
        OTP/report_protocol.h
        OTP/sha1.c
        OTP/sha1.h
        SOFTWARE_FRAMEWORK/BOARDS/EVK1104/evk1104.h
        SOFTWARE_FRAMEWORK/BOARDS/EVK1104/led.c
        SOFTWARE_FRAMEWORK/BOARDS/EVK1104/led.h
        SOFTWARE_FRAMEWORK/BOARDS/board.h
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/DATA_FLASH/AT45DBX/at45dbx.c
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/DATA_FLASH/AT45DBX/at45dbx.h
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/DATA_FLASH/AT45DBX/at45dbx_mem.c
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/DATA_FLASH/AT45DBX/at45dbx_mem.h
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/sd_mmc_cmd.h
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/sd_mmc_mci.c
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/sd_mmc_mci.h
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/sd_mmc_mci_mem.c
        SOFTWARE_FRAMEWORK/COMPONENTS/MEMORY/SD_MMC/SD_MMC_MCI/sd_mmc_mci_mem.h
        SOFTWARE_FRAMEWORK/DRIVERS/ADC/adc.c
        SOFTWARE_FRAMEWORK/DRIVERS/ADC/adc.h
        SOFTWARE_FRAMEWORK/DRIVERS/AES/aes.c
        SOFTWARE_FRAMEWORK/DRIVERS/AES/aes.h
        SOFTWARE_FRAMEWORK/DRIVERS/AES/aes_1231.h
        SOFTWARE_FRAMEWORK/DRIVERS/AES/ram_aes_ram_example.c
        SOFTWARE_FRAMEWORK/DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h
        SOFTWARE_FRAMEWORK/DRIVERS/FLASHC/flashc.c
        SOFTWARE_FRAMEWORK/DRIVERS/FLASHC/flashc.h
        SOFTWARE_FRAMEWORK/DRIVERS/GPIO/gpio.c
        SOFTWARE_FRAMEWORK/DRIVERS/GPIO/gpio.h
        SOFTWARE_FRAMEWORK/DRIVERS/INTC/intc.c
        SOFTWARE_FRAMEWORK/DRIVERS/INTC/intc.h
        SOFTWARE_FRAMEWORK/DRIVERS/MCI/mci.c
        SOFTWARE_FRAMEWORK/DRIVERS/MCI/mci.h
        SOFTWARE_FRAMEWORK/DRIVERS/PM/pm.c
        SOFTWARE_FRAMEWORK/DRIVERS/PM/pm.h
        SOFTWARE_FRAMEWORK/DRIVERS/PM/pm_conf_clocks.c
        SOFTWARE_FRAMEWORK/DRIVERS/PM/power_clocks_lib.c
        SOFTWARE_FRAMEWORK/DRIVERS/PM/power_clocks_lib.h
        SOFTWARE_FRAMEWORK/DRIVERS/SPI/spi.c
        SOFTWARE_FRAMEWORK/DRIVERS/SPI/spi.h
        SOFTWARE_FRAMEWORK/DRIVERS/TC/tc.c
        SOFTWARE_FRAMEWORK/DRIVERS/TC/tc.h
        SOFTWARE_FRAMEWORK/DRIVERS/USART/usart.c
        SOFTWARE_FRAMEWORK/DRIVERS/USART/usart.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_descriptors.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_descriptors.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_device_task.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_device_task.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_specific_request.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_specific_request.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_standard_request.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/DEVICE/usb_standard_request.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/HOST/usb_host_enum.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/HOST/usb_host_enum.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/HOST/usb_host_task.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/HOST/usb_host_task.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/usb_task.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/ENUM/usb_task.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/usb_drv.c
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/usb_drv.h
        SOFTWARE_FRAMEWORK/DRIVERS/USBB/usbb.h
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fat.c
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fat.h
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fat_unusual.c
        SOFTWARE_FRAMEWORK/SERVICES/FAT/file.c
        SOFTWARE_FRAMEWORK/SERVICES/FAT/file.h
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fs_com.h
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fsaccess.c
        SOFTWARE_FRAMEWORK/SERVICES/FAT/fsaccess.h
        SOFTWARE_FRAMEWORK/SERVICES/FAT/navigation.c
        SOFTWARE_FRAMEWORK/SERVICES/FAT/navigation.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/croutine.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/FreeRTOS.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/list.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/mpu_wrappers.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/portable.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/projdefs.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/queue.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/semphr.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/StackMacros.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/include/task.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/portable/GCC/AVR32_UC3/port.c
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/portable/GCC/AVR32_UC3/portmacro.h
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/portable/MemMang/heap_3.c
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/croutine.c
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/list.c
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/queue.c
        SOFTWARE_FRAMEWORK/SERVICES/FREERTOS/Source/tasks.c
        SOFTWARE_FRAMEWORK/SERVICES/MEMORY/CTRL_ACCESS/ctrl_access.c
        SOFTWARE_FRAMEWORK/SERVICES/MEMORY/CTRL_ACCESS/ctrl_access.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/aes.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/arc4.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/base64.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/bignum.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/bn_mul.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/camellia.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/certs.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/config.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/debug.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/des.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/dhm.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/havege.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/md2.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/md4.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/md5.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/net.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/openssl.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/padlock.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/rsa.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/sha1.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/sha2.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/sha4.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/ssl.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/timing.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/x509.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/include/polarssl/xtea.h
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/aes.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/arc4.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/base64.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/bignum.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/camellia.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/certs.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/debug.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/des.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/dhm.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/havege.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/md2.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/md4.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/md5.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/net.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/padlock.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/rsa.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/sha1.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/sha2.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/sha4.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/ssl_cli.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/ssl_srv.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/ssl_tls.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/timing.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/x509parse.c
        SOFTWARE_FRAMEWORK/SERVICES/POLARSSL/xtea.c
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/HID/hid.h
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/HOST_MEM/host_mem.c
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/HOST_MEM/host_mem.h
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/SCSI_DECODER/scsi_decoder.c
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/SCSI_DECODER/scsi_decoder.h
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/device_mass_storage_task.c
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/device_mass_storage_task.h
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/host_mass_storage_task.c
        SOFTWARE_FRAMEWORK/SERVICES/USB/CLASS/MASS_STORAGE/host_mass_storage_task.h
        SOFTWARE_FRAMEWORK/SERVICES/USB/usb_ids.h
        SOFTWARE_FRAMEWORK/UTILS/DEBUG/debug.c
        SOFTWARE_FRAMEWORK/UTILS/DEBUG/debug.h
        SOFTWARE_FRAMEWORK/UTILS/DEBUG/print_funcs.c
        SOFTWARE_FRAMEWORK/UTILS/DEBUG/print_funcs.h
        SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/nlao_cpu.h
        SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/nlao_exceptions.h
        SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/nlao_interrupts.h
        SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/nlao_io.h
        SOFTWARE_FRAMEWORK/UTILS/LIBS/NEWLIB_ADDONS/INCLUDE/nlao_usart.h
        SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR/mrepeat.h
        SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR/preprocessor.h
        SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR/stringz.h
        SOFTWARE_FRAMEWORK/UTILS/PREPROCESSOR/tpaste.h
        SOFTWARE_FRAMEWORK/UTILS/compiler.h
        SOFTWARE_FRAMEWORK/UTILS/conf_isp.h
        SOFTWARE_FRAMEWORK/UTILS/parts.h
        Tools/AD_Test.c
        Tools/AD_Test.h
        Tools/BUFFERED_SIO.c
        Tools/BUFFERED_SIO.h
        Tools/DebugLog.c
        Tools/DebugLog.h
        Tools/DFU_test.c
        Tools/DFU_test.h
        Tools/fastHash.c
        Tools/fastHash.h
        Tools/Inbetriebnahme.c
        Tools/Inbetriebnahme.h
        Tools/Interpreter.c
        Tools/Interpreter.h
        Tools/LED_test.c
        Tools/LED_test.h
        Tools/OTP_Test.c
        Tools/OTP_Test.h
        Tools/SD_Test.c
        Tools/SD_Test.h
        Tools/TIME_MEASURING.c
        Tools/TIME_MEASURING.h
        Tools/xxHash.c
        Tools/xxHash.h
        USB_CCID/USB_CCID.c
        USB_CCID/USB_CCID.h
        USB_CCID/USB_CCID_task.c
        USB_CCID/USB_CCID_task.h
        USER_INTERFACE/file_io.c
        USER_INTERFACE/file_io.h
        USER_INTERFACE/html_io.c
        USER_INTERFACE/html_io.h
        USHELL/ushell_task.c
        USHELL/ushell_task.h
        VIRTUAL_MEM/virtual_mem.c
        VIRTUAL_MEM/virtual_mem.h
        global.h
        nitrokey.c
        nitrokey.h
        tools.c
        tools.h)
