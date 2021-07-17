#!/usr/bin/env python
from PIL import Image

cimg = """#include <stdint.h>

static const uint8_t img[230400] = {
    """

im = Image.open('rainbow.png').convert('RGB')

y = 0
for i in range(im.size[0]*im.size[1]):
    x = i % 240
    r, g, b = im.getpixel((x, y))

    cimg += "{}, {}, {}, ".format(hex(b), hex(g), hex(r))
    #  print(i, x, y)
    if x == 239:
        y = y + 1
        cimg += "\n    "

cimg += "};"
# write file
with open('img.h', 'w+') as f:
    f.write(cimg)
