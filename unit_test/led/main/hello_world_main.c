#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define TOUCH_PIN_UP GPIO_NUM_4    // 定義增加亮度的觸摸感測引腳
#define TOUCH_PIN_DOWN GPIO_NUM_5  // 定義減少亮度的觸摸感測引腳
#define LED_PIN GPIO_NUM_2         // 定義內建LED引腳

static const char *TAG = "touch_example";
static int brightness = 0;         // 初始化亮度
static const int max_brightness = 255; // 最大亮度
static const int step = 25;        // 每次調整的步長

void app_main(void) {
    // 設置觸摸感測引腳為輸入模式，並啟用內建上拉電阻
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << TOUCH_PIN_UP) | (1ULL << TOUCH_PIN_DOWN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);

    // 配置LED PWM通道
    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = LED_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // 配置LED PWM定時器
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT, // 設置PWM占空比解析度為8位
        .freq_hz = 5000,                     // 設置PWM頻率為5kHz
        .speed_mode = LEDC_HIGH_SPEED_MODE,  // 使用高速模式
        .timer_num = LEDC_TIMER_0            // 使用定時器0
    };
    ledc_timer_config(&ledc_timer);

    // 設置串行通信速率
    esp_log_level_set(TAG, ESP_LOG_INFO);

    while (1) {
        int touchValueUp = gpio_get_level(TOUCH_PIN_UP); // 讀取增加亮度的觸摸感測引腳的值
        int touchValueDown = gpio_get_level(TOUCH_PIN_DOWN); // 讀取減少亮度的觸摸感測引腳的值

        ESP_LOGI(TAG, "Touch Value Up: %d, Touch Value Down: %d", touchValueUp, touchValueDown); // 將觸摸值打印到串行監視器

        // 如果增加亮度的感測器被觸摸（輸出為HIGH），則增加LED亮度
        if (touchValueUp == 1) {
            brightness += step;
            if (brightness > max_brightness) {
                brightness = max_brightness;
            }
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, brightness);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        }

        // 如果減少亮度的感測器被觸摸（輸出為HIGH），則減少LED亮度
        if (touchValueDown == 1) {
            brightness -= step;
            if (brightness < 0) {
                brightness = 0;
            }
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, brightness);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);  // 延遲100毫秒
    }
}
