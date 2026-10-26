/*
 * SPDX-FileCopyrightText: acerhizm
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "CameraProviderExtension"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <log/log.h>

// Every rear camera-flash node uses the PMIC torch channel 0 (pm8350c). The qti-flash driver
// takes the brightness in mA and, while the torch is on, only retargets its current.
static const char* kTorchBrightnessPath = "/sys/class/leds/led:torch_0/brightness";

// Current the camera HAL uses for the torch on stock HyperOS, and the channel's max-current-ma.
static constexpr int32_t kTorchDefaultStrength = 160;
static constexpr int32_t kTorchMaxStrength = 500;

static std::atomic<int32_t> sTorchStrength{kTorchDefaultStrength};

static void writeTorchBrightness(int32_t milliamps) {
    int fd = open(kTorchBrightnessPath, O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        ALOGE("Failed to open %s: %d", kTorchBrightnessPath, errno);
        return;
    }
    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%d", milliamps);
    if (write(fd, buf, len) != len) {
        ALOGE("Failed to write %d to %s: %d", milliamps, kTorchBrightnessPath, errno);
    }
    close(fd);
}

bool supportsTorchStrengthControlExt() {
    return true;
}

int32_t getTorchDefaultStrengthLevelExt() {
    return kTorchDefaultStrength;
}

int32_t getTorchMaxStrengthLevelExt() {
    return kTorchMaxStrength;
}

int32_t getTorchStrengthLevelExt() {
    return sTorchStrength.load();
}

void setTorchStrengthLevelExt(int32_t torchStrength, bool enabled) {
    if (!enabled) {
        // The HAL has already switched the torch off. Never write 0 here: the LED core drops
        // the camera driver's torch trigger on a 0 write, and the torch would stay dead. Any
        // non-zero write while off would light the LED again.
        sTorchStrength.store(kTorchDefaultStrength);
        return;
    }

    if (torchStrength > kTorchMaxStrength) torchStrength = kTorchMaxStrength;
    if (torchStrength < 1) torchStrength = 1;

    sTorchStrength.store(torchStrength);
    writeTorchBrightness(torchStrength);
}

std::vector<std::pair<std::string, uint8_t>> getCaptureRequestU8OverridesExt(
        const std::string& /* cameraId */, const std::string& /* packageName */) {
    return {};
}
