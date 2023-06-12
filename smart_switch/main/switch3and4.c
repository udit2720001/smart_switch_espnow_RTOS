/* ESPNOW Example - Receive "Hello World"

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdlib.h>
#include <string.h>
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "espnow_example.h"
#include "esp_mac.h"
#include "esp_crc.h"
#include "esp_random.h"
#include "driver/gpio.h"

#define SW3 GPIO_NUM_26 // sw3
#define SW4 GPIO_NUM_27 // sw4

#define ESPNOW_MAXDELAY 512

const uint8_t *data;

int recdata;

static const char *TAG = "Smart Switch";
static uint8_t YOUR_PEER_MAC_ADDRESS[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
/* WiFi should start before using ESPNOW */
static void example_wifi_init(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/* ESPNOW data receiving callback function */
static void example_espnow_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    ESP_LOGI(TAG, "Received data from " MACSTR ", len=%d", MAC2STR(mac_addr), len);
    recdata = (int)(*data);

    ESP_LOGI(TAG, "Data callback %d", recdata);
}
/* ESPNOW sending callback function */
static void example_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    ESP_LOGI(TAG, "Send status: %d", status);
}

void app_main()
{
    esp_err_t ret;

    gpio_set_direction(SW3, GPIO_MODE_OUTPUT);
    gpio_set_level(SW3, 1);

    gpio_set_direction(SW4, GPIO_MODE_OUTPUT);
    gpio_set_level(SW4, 1);

    ret = nvs_flash_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize NVS");
        return;
    }
    //

    // Initialize WiFi
    example_wifi_init();

    // Initialize ESPNOW
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(example_espnow_recv_cb));

    // Add broadcast peer (optional)
    esp_now_peer_info_t broadcast_peer;
    memset(broadcast_peer.peer_addr, 0xFF, ESP_NOW_ETH_ALEN);
    broadcast_peer.channel = CONFIG_ESPNOW_CHANNEL;
    broadcast_peer.ifidx = ESPNOW_WIFI_IF;
    broadcast_peer.encrypt = false;

    ret = esp_now_add_peer(&broadcast_peer);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add broadcast peer: %d", ret);
        return;
    }
    printf("Init done \n");

    ESP_LOGI(TAG, "Waiting for data...");
    printf("Waiting... \n");

    while (true)
    {
        if (recdata == 00)
        {
            ESP_LOGI(TAG, "Idle mode wating for data: %d", recdata);
        }
        else if (recdata == 5)
        {
            ESP_LOGI(TAG, "Switch 3 is off. %d", recdata);
            gpio_set_level(SW3, 1);
        }
        else if (recdata == 6)
        {
            ESP_LOGI(TAG, "Switch 3 is on. %d", recdata);
            gpio_set_level(SW3, 0);
        }
        else if (recdata == 7)
        {
            ESP_LOGI(TAG, "Switch 4 is off. %d", recdata);
            gpio_set_level(SW4, 1);
        }
        else if (recdata == 8)
        {
            ESP_LOGI(TAG, "Switch 4 is on. %d", recdata);
            gpio_set_level(SW4, 0);
        }
        else if (recdata == 15)
        {
            ESP_LOGI(TAG, "All Switch off %d", recdata);
            gpio_set_level(SW3, 1);
            gpio_set_level(SW4, 1);
        }
        else
        {
            ESP_LOGI(TAG, "Hoping through next : %d", recdata);
            ret = esp_now_send(broadcast_peer.peer_addr, &recdata, sizeof(recdata));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }

            ESP_LOGI(TAG, "Data sent successfully");
        }

        vTaskDelay(ESPNOW_MAXDELAY / portTICK_PERIOD_MS);
    }
}