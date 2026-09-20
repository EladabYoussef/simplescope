#include "display.h"

#include <stdio.h>
#include <string.h>

static int failures;
static int update_calls;

int ssd1306_update_screen(void)
{
    update_calls++;
    return 0;
}

static void expect(int condition, const char *message)
{
    if (!condition) {
        printf("FAIL: %s\n", message);
        failures++;
    }
}

static int pixel_is_set(uint8_t x, uint8_t y)
{
    return (FRAME_BUFFER[y][x / 8u] & (uint8_t)(1u << (x % 8u))) != 0;
}

static void test_channel_selection(void)
{
    uint16_t data[N_CHANNELS * SAMPLES_PER_CHANNEL];

    for (uint32_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        data[i] = (i % 2u == 0u) ? 0u : 4095u;
    }

    display_signal(data, 256u, CHANNEL_1);
    expect(update_calls == 1, "channel 1 updates the OLED");
    expect(pixel_is_set(0, OLED_H - 1u),
           "channel 1 uses even input indices and maps zero to the bottom");
    expect(pixel_is_set(OLED_W - 1u, OLED_H - 1u),
           "channel 1 renders its final even-indexed sample");
    expect(!pixel_is_set(OLED_W / 2u, 0),
           "channel 1 does not render channel 2 values");

    display_signal(data, 256u, CHANNEL_2);
    expect(update_calls == 2, "channel 2 updates the OLED");
    expect(pixel_is_set(0, 0),
           "channel 2 uses odd input indices and maps full scale to the top");
    expect(pixel_is_set(OLED_W - 1u, 0),
           "channel 2 renders its final odd-indexed sample");
}

static void test_invalid_input(void)
{
    int calls_before = update_calls;

    memset(FRAME_BUFFER, 0, sizeof(FRAME_BUFFER));
    FRAME_BUFFER[0][0] = 0xA5;

    display_signal(NULL, 256u, CHANNEL_1);
    display_signal((const uint16_t *)FRAME_BUFFER, 0u, CHANNEL_1);
    display_signal((const uint16_t *)FRAME_BUFFER, 256u, NO_CHANNEL_SELECTED);

    expect(update_calls == calls_before,
           "invalid input does not update the OLED");
    expect(FRAME_BUFFER[0][0] == 0xA5,
           "invalid input does not alter the framebuffer");
}

int main(void)
{
    test_channel_selection();
    test_invalid_input();

    if (failures == 0) {
        printf("PASS: all display tests passed\n");
        return 0;
    }

    printf("FAIL: %d test(s) failed\n", failures);
    return 1;
}
