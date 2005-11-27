#!/bin/bash

#files="sand.png small_rough_earth.png grass_light.png grass_small.png"
files="rough_earth.png earth_stones.png grass_flower.png big_grass.png"
montage +frame +shadow -geometry 128x128+0+0 -tile 1x4 $files new3d.png
#montage +frame +shadow -geometry 64x64+0+0 -tile 1x4 $files new3d.png
