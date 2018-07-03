import numpy as np
import sys
import cv2
import matplotlib.pyplot as plt
from scipy.signal import argrelextrema

img_src_filename = 'typo.png'
img_src = cv2.imread(img_src_filename, 0);
height, width_total = img_src.shape

filter_detector = [1, -2, 1]
changes = np.convolve(img_src[-2,:], filter_detector, 'same')
edges, = argrelextrema(changes, np.greater)
edges = np.concatenate(([0], edges, [width_total-1]), 0)

positions = open('letter_positions.dat', 'w')
positions.write('CODE\tMIN_X\tMAX_X\tMIN_Y\tMAX_Y\n')
for i, edge in enumerate(edges[1:]):
    img_char = img_src[1:, edges[i]:edges[i+1]]
    
    condition_y, condition_x = np.where(img_char[4:-4,4:-4] < 127)
    if (len(condition_y)>0):
        max_x = np.max(condition_x) + 4
        min_x = np.min(condition_x) + 4
        max_y = np.max(condition_y) + 4
        min_y = np.min(condition_y) + 4
    else:
        max_x = edges[i+1]-4
        min_x = 4
        max_y = height-5
        min_y = 4
    
    positions.write('{}\t{}\t{}\t{}\t{}\n'.format(i+32, min_x, max_x, min_y, max_y))
    
    filename = 'char_' + str(i+32) + '.png'
    cv2.imwrite(filename, img_char)

positions.close()
