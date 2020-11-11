#include "main.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#define LOG_TAG "MAIN"

//#define TOGGLE_RELAY
static Main main_class{};

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
extern "C" void app_main(void)
{
    ESP_LOGD(LOG_TAG, "Creating default event loop");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGD(LOG_TAG, "Initialising NVS");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_LOGI(LOG_TAG, "Running setup");
    while (!main_class.setup())
    {
        vTaskDelay(100);
        ESP_LOGD(LOG_TAG, "Re-running setup");
    }

    ESP_LOGI(LOG_TAG, "Setup complete, entering main loop");

    while (true)
    {
        main_class.run();
    }
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

Bt_Le::Ble &Main::ble{Bt_Le::Ble::get_instance(ESP_BT_MODE_BLE)};
SNTP::Sntp &Main::sntp{SNTP::Sntp::get_instance()};

// --------------------------------------------------------------------------------
[[nodiscard]] bool Main::setup(void)
{
    esp_err_t status{ESP_OK};

    status |= start_all_tasks();

    return status == ESP_OK;
}

// --------------------------------------------------------------------------------
void Main::run(void)
{
#if defined(MEMORY_DEBUGGING)
    log_mem();
    vTaskDelay(pdMS_TO_TICKS(MEMORY_LOG_INTERVAL_MS));
#elif defined(TOGGLE_RELAY)
    static bool flipflop = false;
    relay.set(!relay.get());
    ESP_LOGD(LOG_TAG, "GPIO state: %s", relay.get() ? "HIGH" : "LOW");
    flipflop = !flipflop;
    vTaskDelay((10 * 60 * 1000) / portTICK_PERIOD_MS);
#else

    // Main user run code
    vTaskDelay(portMAX_DELAY);
#endif
}

// --------------------------------------------------------------------------------
// Function to start tasks by notification
[[nodiscard]] esp_err_t Main::start_all_tasks(void)
{
    esp_err_t ret_status{ESP_OK};

    sntp.begin();

    while (!sntp.got_time())
    {
        ESP_LOGD(LOG_TAG, "Waiting for NTP time server");
        vTaskDelay(pdSECOND);
    }

    ble.begin();

    return ret_status;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------