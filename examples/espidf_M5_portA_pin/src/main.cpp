#include "M5_FPC1020A.h"
#include "esp_log.h"
#include <M5Unified.h>

const char TAG[] = "main";

void test_task(void *pvParameter) {
  FingerPrint fingerprint;
  fingerprint.begin(UART_NUM_2, M5.getPin(m5::port_a_pin2),
                    M5.getPin(m5::port_a_pin1), UART_PIN_NO_CHANGE,
                    UART_PIN_NO_CHANGE);
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  if (fingerprint.fpm_deleteAllUser() == ACK_SUCCESS) {
    ESP_LOGI(TAG, "Sucess to delete all user");
  } else {
    ESP_LOGE(TAG, "Fail to delete all user");
  }

  ESP_LOGI(TAG, "Reading Finger to Save");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  if (fingerprint.fpm_addUser(0, 1) == ACK_SUCCESS) {
    ESP_LOGI(TAG, "Sucess to save user");
  } else {
    ESP_LOGE(TAG, "Fail to dsave user");
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  while (1) {
    ESP_LOGI(TAG, "Reading Finger");
    if (fingerprint.fpm_compareFinger() == ACK_SUCCESS) {
      auto id = fingerprint.fpm_getUserId();
      ESP_LOGI(TAG, "Found User: %d", id);
    } else {
      ESP_LOGE(TAG, "User not Found");
    }
    vTaskDelay(1000);
  }
}

extern "C" void app_main(void) {
  M5.begin();
  xTaskCreate(&test_task, "test_task", 3 * 1024, nullptr, 5, nullptr);
  while (1) {
    M5.update();
    M5.delay(10);
  }
}