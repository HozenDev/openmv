static const char fresh_main_py[] =
    "# This work is licensed under the MIT license.\n"
    "# Copyright (c) 2013-2024 OpenMV LLC. All rights reserved.\n"
    "# https://github.com/openmv/openmv/blob/master/LICENSE\n"
    "#\n"
    "# Pure Thermal Example Script\n"
    "#\n"
    "# Thanks for buying the Pure Thermal OpenMV! This example script shows off thermal video\n"
    "# overlay onto the color camera image and driving the attached LCD screen and HDMI output.\n"
    "\n"
    "import sensor\n"
    "import image\n"
    "import time\n"
    "import display\n"
    "import fir\n"
    "import math\n"
    "import tfp410\n"
    "\n"
    "\n"
    "# Color Tracking Thresholds (Grayscale Min, Grayscale Max)\n"
    "threshold_list = [(200, 255)]\n"
    "\n"
    "sensor.reset()\n"
    "sensor.set_pixformat(sensor.RGB565)\n"
    "sensor.set_framesize(sensor.WVGA)\n"
    "time.sleep_ms(50)\n"
    "\n"
    "fir.init(fir.FIR_LEPTON)\n"
    "fir_img = image.Image(fir.width(), fir.height(), sensor.GRAYSCALE)\n"
    "time.sleep_ms(50)\n"
    "\n"
    "lcd = display.RGBDisplay(framesize=display.FWVGA, refresh=60)\n"
    "lcd.backlight(True)\n"
    "hdmi = tfp410.TFP410()\n"
    "time.sleep_ms(50)\n"
    "\n"
    "alpha_pal = image.Image(256, 1, sensor.GRAYSCALE)\n"
    "for i in range(256):\n"
    "    alpha_pal[i] = int(math.pow((i / 255), 2) * 255)\n"
    "\n"
    "to_min = None\n"
    "to_max = None\n"
    "\n"
    "\n"
    "def map_g_to_temp(g):\n"
    "    return ((g * (to_max - to_min)) / 255.0) + to_min\n"
    "\n"
    "\n"
    "while True:\n"
    "    img = sensor.snapshot()\n"
    "    # ta: Ambient temperature\n"
    "    # ir: Object temperatures (IR array)\n"
    "    # to_min: Minimum object temperature\n"
    "    # to_max: Maximum object temperature\n"
    "    ta, ir, to_min, to_max = fir.read_ir()\n"
    "\n"
    "    fir.draw_ir(fir_img, ir, color_palette=None)\n"
    "    fir_img_size = fir_img.width() * fir_img.height()\n"
    "\n"
    "    # Find IR Blobs\n"
    "    blobs = fir_img.find_blobs(threshold_list,\n"
    "                               pixels_threshold=(fir_img_size // 100),\n"
    "                               area_threshold=(fir_img_size // 100),\n"
    "                               merge=True)\n"
    "\n"
    "    # Collect stats into a list of tuples\n"
    "    blob_stats = []\n"
    "    for b in blobs:\n"
    "        blob_stats.append((b.rect(), map_g_to_temp(img.get_statistics(thresholds=threshold_list,\n"
    "                                                                      roi=b.rect()).mean())))\n"
    "    x_scale = img.width() / fir_img.width()\n"
    "    y_scale = img.height() / fir_img.height()\n"
    "    img.draw_image(fir_img, 0, 0, x_scale=x_scale, y_scale=y_scale,\n"
    "                   color_palette=image.PALETTE_IRONBOW,\n"
    "                   alpha_palette=alpha_pal,\n"
    "                   hint=image.BICUBIC)\n"
    "\n"
    "    # Draw stuff on the colored image\n"
    "    for b in blobs:\n"
    "        img.draw_rectangle(int(b.rect()[0] * x_scale), int(b.rect()[1] * y_scale),\n"
    "                           int(b.rect()[2] * x_scale), int(b.rect()[3] * y_scale))\n"
    "        img.draw_cross(int(b.cx() * x_scale), int(b.cy() * y_scale))\n"
    "    for blob_stat in blob_stats:\n"
    "        img.draw_string(int((blob_stat[0][0] * x_scale) + 4), int((blob_stat[0][1] * y_scale) + 1),\n"
    "                        '%.2f C' % blob_stat[1], mono_space=False, scale=2)\n"
    "\n"
    "    # Draw ambient, min and max temperatures.\n"
    "    img.draw_string(4, 0, 'Lepton Temp: %0.2f C' % ta,\n"
    "                    color=(255, 255, 255), mono_space=False, scale=2)\n"
    "    img.draw_string(4, 18, 'Min Temp: %0.2f C' % to_min,\n"
    "                    color=(255, 255, 255), mono_space=False, scale=2)\n"
    "    img.draw_string(4, 36, 'Max Temp: %0.2f C' % to_max,\n"
    "                    color=(255, 255, 255), mono_space=False, scale=2)\n"
    "\n"
    "    lcd.write(img, hint=(image.BILINEAR | image.CENTER | image.SCALE_ASPECT_KEEP))\n"
    "\n"
;
