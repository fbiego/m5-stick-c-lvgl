/*
   MIT License

  Copyright (c) 2024 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ______________  _____
  ___  __/___  /_ ___(_)_____ _______ _______
  __  /_  __  __ \__  / _  _ \__  __ `/_  __ \
  _  __/  _  /_/ /_  /  /  __/_  /_/ / / /_/ /
  /_/     /_.___/ /_/   \___/ _\__, /  \____/
                              /____/

*/

#include <Arduino.h>

#include <Timber.h>
#include <lvgl.h>
#include <M5Unified.h>

// landscape screen config
static const uint32_t screenWidth = 160;
static const uint32_t screenHeight = 80;

const unsigned int lvBufferSize = screenWidth * screenHeight / 4;
uint8_t lvBuffer[2][lvBufferSize];

void my_disp_flush(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  lv_draw_sw_rgb565_swap(data, w * h);

  M5.Lcd.pushImageDMA<uint16_t>(area->x1, area->y1, w, h, (uint16_t *)data);

  lv_display_flush_ready(display); /* tell lvgl that flushing is done */
}

void logCallback(Level level, unsigned long time, String message)
{
  Serial.print(message);
}

static uint32_t my_tick(void)
{
  return millis();
}

void setup()
{
  Serial.begin(115200);
  Timber.setLogCallback(logCallback);

  M5.begin();
  // M5.Power.ScreenBreath(8);
  // setCpuFrequencyMhz(10);
  M5.Lcd.setRotation(1); // make the display landscape
  // M5.Imu.init();

  lv_init();

  lv_tick_set_cb(my_tick);

  static auto *lvDisplay = lv_display_create(screenWidth, screenHeight);
  lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(lvDisplay, my_disp_flush);

  lv_display_set_buffers(lvDisplay, lvBuffer[0], lvBuffer[1], lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_obj_t *label1 = lv_label_create(lv_screen_active());
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(label1, screenWidth - 30);
  lv_label_set_text(label1, "M5 Stick C LVGL v9");

  Timber.i("Setup done");
  


}

void loop()
{
  M5.update();

  lv_timer_handler(); /* let the GUI do its work */
  delay(5);

}