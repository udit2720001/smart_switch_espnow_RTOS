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

#define ESPNOW_MAXDELAY 512

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

/* ESPNOW sending callback function */
static void example_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    ESP_LOGI(TAG, "Send status: %d", status);
}

void app_main()
{
    esp_err_t ret;
    gpio_set_direction(GPIO_NUM_21, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_21, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_19, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GPIO_NUM_18, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_18, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_17, GPIO_PULLDOWN_ONLY);

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize NVS");
        return;
    }

    // Initialize WiFi
    example_wifi_init();

    // Initialize ESPNOW
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(example_espnow_send_cb));

    // Add peer
    esp_now_peer_info_t peer;
    memcpy(peer.peer_addr, YOUR_PEER_MAC_ADDRESS, ESP_NOW_ETH_ALEN);
    peer.channel = CONFIG_ESPNOW_CHANNEL;
    peer.ifidx = ESPNOW_WIFI_IF;
    peer.encrypt = false;

    ret = esp_now_add_peer(&peer);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add peer: %d", ret);
        return;
    }

    int data = 0;
    while (true)
    {

        if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 0; // Send the integer value 0
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
            ESP_LOGI(TAG, "Send Data %d", data);
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 1; // Send the integer value 1
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 2; // Send the integer value 2
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 3; // Send the integer value 3
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 4; // Send the integer value 4
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 5; // Send the integer value 5
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 6; // Send the integer value 6
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 0 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 7; // Send the integer value 7
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 8; // Send the integer value 8
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 9; // Send the integer value 9
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 10; // Send the integer value 10
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 0 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 11; // Send the integer value 11
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 12; // Send the integer value 12
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 0 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 13; // Send the integer value 13
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 0)
        {
            // Send data
            data = 14; // Send the integer value 14
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else if (gpio_get_level(GPIO_NUM_21) == 1 && gpio_get_level(GPIO_NUM_19) == 1 && gpio_get_level(GPIO_NUM_18) == 1 && gpio_get_level(GPIO_NUM_17) == 1)
        {
            // Send data
            data = 15; // Send the integer value 15
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
        }
        else
        {
            // Send data
            data = 69; // Send the integer value 16
            ret = esp_now_send(peer.peer_addr, &data, sizeof(data));
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to send data: %d", ret);
                return;
            }
            ESP_LOGE(TAG, "Sorry you are running out of switches please upgrade your plan %d", data);
        }
        ESP_LOGI(TAG, "sent data: %d", data);

        vTaskDelay(ESPNOW_MAXDELAY / portTICK_PERIOD_MS);
    }
}
