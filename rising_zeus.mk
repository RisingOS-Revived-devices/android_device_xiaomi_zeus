#
# Copyright (C) 2022-2024 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from zeus device
$(call inherit-product, device/xiaomi/zeus/device.mk)

# Rising flags
RISING_MAINTAINER := acerhizm
RISING_CHIPSET := Snapdragon® 8 Gen 1
WITH_GMS := true
PRODUCT_BROKEN_VERIFY_USES_LIBRARIES := true
TARGET_ENABLE_BLUR := true
TARGET_INCLUDE_LIVE_WALLPAPERS := true
TARGET_SUPPORTS_GOOGLE_RECORDER := true
TARGET_SUPPORTS_QUICK_TAP := true
PRODUCT_OTA_ENFORCE_VINTF_KERNEL_REQUIREMENTS := false
PERF_ANIM_OVERRIDE := true
WITH_BCR := true

# MIUI camera replaces Aperture, where the tree has it
ifneq ($(wildcard device/xiaomi/miuicamera-cupid/device.mk),)
PRODUCT_NO_CAMERA := true
endif

# Inherit some common RisingOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Boot animation
TARGET_SCREEN_HEIGHT := 3200
TARGET_SCREEN_WIDTH := 1440
TARGET_BOOT_ANIMATION_RES := 1440

PRODUCT_NAME := rising_zeus
PRODUCT_DEVICE := zeus
PRODUCT_MANUFACTURER := Xiaomi
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := 2201122G

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="zeus_global-user 15 AQ3A.250226.002 OS3.0.4.0.VLBMIXM release-keys" \
    BuildFingerprint=Xiaomi/zeus_global/zeus:15/AQ3A.250226.002/OS3.0.4.0.VLBMIXM:user/release-keys \
    DeviceProduct=zeus \
    SystemName=zeus_global \
    RisingChipset="Snapdragon® 8 Gen 1" \
    RisingMaintainer="acerhizm"

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi
