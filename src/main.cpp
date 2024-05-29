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

#define SCR 5

// landscape screen config
static const uint32_t screenWidth = 160;
static const uint32_t screenHeight = 80;

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

static lv_color_t disp_draw_buf[screenWidth * SCR];
static lv_color_t disp_draw_buf2[screenWidth * SCR];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  uint32_t wh = w * h;

  M5.Lcd.startWrite();
  M5.Lcd.setAddrWindow(area->x1, area->y1, w, h);
  while (wh--)
    M5.Lcd.pushColor(color_p++->full);
  M5.Lcd.endWrite();

  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

void logCallback(Level level, unsigned long time, String message)
{
  Serial.print(message);
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

  if (!disp_draw_buf)
  {
    Timber.e("LVGL disp_draw_buf allocate failed!");
  }
  else
  {

    Timber.i("Display buffer size initialized ");

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, disp_draw_buf2, screenWidth * SCR);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label1, screenWidth - 30);
    lv_label_set_text(label1, "M5 Stick C LVGL");

    // /*Create a slider below the label*/
    // lv_obj_t *slider1 = lv_slider_create(lv_scr_act());
    // lv_obj_set_width(slider1, screenWidth - 40);
    // lv_obj_align_to(slider1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

    Timber.i("Setup done");
  }


}

void loop()
{
  M5.update();

  lv_timer_handler(); /* let the GUI do its work */

  delay(5);

}