#include <chrono>
#include <cstdlib>

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define pdSECOND pdMS_TO_TICKS(1000)

#include "driver/gpio.h"

#include "esp_event.h"
#include "nvs_flash.h"

#include "Ble.h"
#include "SntpTime.h"

// --------------------------------------------------------------------------------------------------------------------
class Main final // To ultimately become the co-ordinator if deemed necessary
{
public:
    constexpr Main(void) {}

    [[nodiscard]] bool setup(void);
    void run(void);

    [[nodiscard]] esp_err_t start_all_tasks(void);

    static Bt_Le::Ble &ble;
    static SNTP::Sntp &sntp;

#if defined(MEMORY_DEBUGGING)
    void log_mem(void);
#endif
}; // class Main

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------