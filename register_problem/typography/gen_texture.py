import numpy as np
import sys
import cv2
import matplotlib.pyplot as plt
from scipy.signal import argrelextrema

characters_count = 224
sample = cv2.imread('char_32.png', 0)
height, width = sample.shape
height = int(np.floor(width * 1.75))
width = width + 1
character_map = 255*np.ones([height * 15, width * 15])

offset_x = 0
offset_y = 0
for i in range(characters_count):
    if (i > 0 and i%15 == 0):
        print('New line -------------------')
        offset_y = offset_y + height
        offset_x = 0
    
    filename = 'char_' + str(i + 32) + '.png'
    current_char = cv2.imread(filename, 0)
    curr_height, curr_width = current_char.shape
    current_char[current_char > 127] = 255
    print('Loading character from: {}, with shape: height = {}, width = {}'.format(filename, height, width))
    character_map[offset_y:(offset_y + curr_height), offset_x:(offset_x + curr_width)] = current_char
    
    offset_x = offset_x + width
    

plt.imshow(character_map, 'gray')
plt.show()

cv2.imwrite('character_texture_map.png', character_map)
