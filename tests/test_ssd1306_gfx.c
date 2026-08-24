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

static int buffer_is_zero(const uint8_t *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (buffer[i] != 0) {
            return 0;
        }
    }
    return 1;
}

static size_t count_set_pixels(void)
{
    size_t count = 0;
    for (size_t row = 0; row < OLED_H; row++) {
        for (size_t column = 0; column < OLED_W_BYTES; column++) {
            uint8_t byte = FRAME_BUFFER[row][column];
            while (byte != 0) {
                count += byte & 1u;
                byte >>= 1;
            }
        }
    }
    return count;
}

static void test_clear_frame_buffer(void)
{
    memset(FRAME_BUFFER, 0xFF, sizeof(FRAME_BUFFER));
    memset(SSD_FRAME_BUFFER, 0xFF, sizeof(SSD_FRAME_BUFFER));
    clear_frame_buffer();

    expect(buffer_is_zero(&FRAME_BUFFER[0][0], sizeof(FRAME_BUFFER)),
           "clear_frame_buffer clears the drawing buffer");
    expect(buffer_is_zero(&SSD_FRAME_BUFFER[0][0], sizeof(SSD_FRAME_BUFFER)),
           "clear_frame_buffer clears the SSD1306 buffer");
}

static void test_draw_bitmap(void)
{
    static const uint8_t bitmap[] = {0x21, 0x42};

    clear_frame_buffer();
    expect(draw_bitmap(0, 3, 8, 2, bitmap) == SSD1306_OK,
           "draw_bitmap accepts an in-bounds bitmap");
    expect(FRAME_BUFFER[3][0] == 0x21 && FRAME_BUFFER[4][0] == 0x42,
           "draw_bitmap copies aligned bitmap rows");
    expect(draw_bitmap(0, 0, 1, 1, NULL) == SSD1306_ERR_INVALID_CHAR,
           "draw_bitmap rejects a null bitmap");
    expect(draw_bitmap(OLED_W, 0, 1, 1, bitmap) == SSD1306_ERR_CLIPPED,
           "draw_bitmap rejects an x coordinate outside the display");
    expect(draw_bitmap(0, OLED_H, 1, 1, bitmap) == SSD1306_ERR_CLIPPED,
           "draw_bitmap rejects a y coordinate outside the display");

    clear_frame_buffer();
    expect(draw_bitmap(OLED_W - 4, OLED_H - 1, 8, 2, bitmap) == SSD1306_ERR_CLIPPED,
           "draw_bitmap reports clipping at the display edge");
}

static void test_draw_text(void)
{
    char text[] = "Hello";
    char invalid[] = "\n";

    clear_frame_buffer();
    expect(draw_text(0, 0, text, strlen(text), FONT_CGA_16) == SSD1306_OK,
           "draw_text renders a valid string");
    expect(count_set_pixels() > 0, "draw_text changes the framebuffer");

    clear_frame_buffer();
    expect(draw_text(0, 0, invalid, 1, FONT_CGA_16) == SSD1306_ERR_INVALID_CHAR,
           "draw_text reports unsupported characters");
    expect(count_set_pixels() == 0, "unsupported characters do not draw pixels");
    expect(draw_text(0, 0, text, strlen(text), FONT_COUNT) == SSD1306_ERR_INVALID_CHAR,
           "draw_text rejects an invalid font id");
    expect(draw_text(OLED_W - 1, 0, text, strlen(text), FONT_CGA_16) == SSD1306_ERR_CLIPPED,
           "draw_text rejects a starting position that cannot fit one glyph");
}

static void test_frame_buffer_conversion(void)
{
    uint8_t expected[PAGES_NUMBER][OLED_W] = {{0}};
    size_t mismatch_count = 0;

    clear_frame_buffer();
    memset(SSD_FRAME_BUFFER, 0xA5, sizeof(SSD_FRAME_BUFFER));
    for (uint8_t row = 0; row < OLED_H; row++) {
        for (uint8_t byte = 0; byte < OLED_W_BYTES; byte++) {
            FRAME_BUFFER[row][byte] = (uint8_t)(1u << ((row + byte) % 8u));
        }
    }

    for (uint8_t page = 0; page < PAGES_NUMBER; page++) {
        for (uint8_t column = 0; column < OLED_W; column++) {
            for (uint8_t bit = 0; bit < 8; bit++) {
                uint8_t source = FRAME_BUFFER[page * 8u + bit][column / 8u];
                expected[page][column] |= ((source >> (column % 8u)) & 1u) << bit;
            }
        }
    }

    convert_frame_buffer_to_ssd_frame_buffer();
    for (uint8_t page = 0; page < PAGES_NUMBER; page++) {
        for (uint8_t column = 0; column < OLED_W; column++) {
            if (expected[page][column] != SSD_FRAME_BUFFER[page][column]) {
                if (mismatch_count == 0) {
                    printf("conversion mismatch at page %u, column %u: expected 0x%02X, got 0x%02X\n",
                           page, column, expected[page][column], SSD_FRAME_BUFFER[page][column]);
                }
                mismatch_count++;
            }
        }
    }
    expect(mismatch_count == 0,
           "conversion maps every framebuffer pixel to its SSD1306 page byte");
    if (mismatch_count != 0) {
        printf("conversion mismatches: %zu\n", mismatch_count);
    }
}

int main(void)
{
    test_clear_frame_buffer();
    test_draw_bitmap();
    test_draw_text();
    test_frame_buffer_conversion();

    if (failures == 0) {
        printf("PASS: all SSD1306 graphics tests passed\n");
        return 0;
    }

    printf("FAIL: %d test(s) failed\n", failures);
    return 1;
}
