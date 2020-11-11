set(SOURCES ./Src/main.cpp ./Src/heap.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . Inc
                        REQUIRES freertos esp_common heap esp_event nvs_flash
                                    esp32_ble esp32_wifi)