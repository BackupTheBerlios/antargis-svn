#!/bin/bash

#files="sand.png small_rough_earth.png grass_light.png grass_small.png"
files="rough_earth.png earth_stones.png grass_flower.png big_grass.png"
#files="rough_earth.png earth_stones.png grass_flower.png ../new_grass3.png"

files="ground.png ground.png big_ground.png big_ground.png big_grass.png big_grass.png big_grass_2.png big_grass_2.png"
files="ground.png ground.png ground_4.png ground_4.png big_grass.png big_grass.png big_grass_2.png big_grass_2.png"


files="ground.png ground.png ground.png ground.png ground_4.png ground_4.png ground_4.png ground_4.png big_grass.png big_grass.png big_grass.png big_grass.png big_grass_2.png big_grass_2.png big_grass_2.png big_grass_2.png"

files="terrain_tile0.png terrain_tile0.png terrain_tile1.png terrain_tile2.png terrain_tile3.png terrain_tile4.png terrain_tile4.png terrain_tile4.png" 
files="terrain_tile*.png"
# terrain_tile4.png terrain_tile4.png terrain_tile4.png terrain_tile4.png"

#files="ground.png ground2.png grass_flower.png big_grass.png"

montage +frame +shadow -geometry 128x128+0+0 -tile 1x8 $files new3d.png
#montage +frame +shadow -geometry 64x64+0+0 -tile 1x4 $files new3d.png
