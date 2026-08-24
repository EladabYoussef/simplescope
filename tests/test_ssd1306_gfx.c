#include "ssd1306_gfx.h"

#include <stdio.h>
#include <string.h>

static int failures;

static void expect(int condition, const char *message)
{
    if (!condition) {
        printf("FAIL: %s\n", message);
        failures++;
    }
}

static size_t count_set_pixels(void)
{
    size_t count = 0;

    for (size_t row = 0; row < OLED_H; row++) {
        for (size_t column = 0; column < OLED_W / 8; column++) {
            uint8_t byte = FRAME_BUFFER[row][column];
            while (byte != 0) {
                count += byte & 1u;
                byte >>= 1;
            }
        }
    }

    return count;
}

static void print_text(void)
{
    printf("Rendered \"Hello STM32\" (128x32):\n");

    for (size_t row = 0; row < 32; row++) {
        for (size_t column = 0; column < OLED_W; column++) {
            uint8_t pixel = (FRAME_BUFFER[row][column / 8] >> (column % 8)) & 1u;
            putchar(pixel ? '#' : '.');
        }
        putchar('\n');
    }
}

int main(void)
{
    uint8_t first_glyph[sizeof(FRAME_BUFFER)];

    clear_test_framebuffer();
    char text[] = "Hello STM32";
    uint8_t status = draw_text(0, 0, text, strlen(text), FONT_CGA_16);
    size_t set_pixels = count_set_pixels();

    print_text();
    printf("draw_text(\"%s\") status: %u\n", text, status);
    printf("set pixels: %zu\n", set_pixels);

    expect(status == SSD1306_OK, "drawing a valid string returns OK");
    expect(set_pixels > 0, "drawing a valid string changes the framebuffer");
    memcpy(first_glyph, FRAME_BUFFER, sizeof(first_glyph));

    clear_frame_buffer();
    char different_text[] = "Goodbye";
    status = draw_text(0, 0, different_text, strlen(different_text), FONT_CGA_16);
    expect(status == SSD1306_OK, "drawing a second valid string returns OK");
    expect(memcmp(first_glyph, FRAME_BUFFER, sizeof(first_glyph)) != 0,
           "different strings produce different framebuffer output");

    clear_frame_buffer();
    char invalid[] = "\n";
    status = draw_text(0, 0, invalid, 1, FONT_CGA_16);
    expect(status == SSD1306_ERR_INVALID_CHAR, "invalid characters are reported");
    expect(count_set_pixels() == 0, "invalid characters do not draw pixels");

    if (failures == 0) {
        printf("PASS: all draw_text tests passed\n");
        return 0;
    }

    printf("FAIL: %d test(s) failed\n", failures);
    return 1;
}