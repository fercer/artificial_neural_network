import numpy as np
import sys
import cv2
import matplotlib.pyplot as plt
from scipy.signal import argrelextrema

img_src_filename = 'texture_map.png'
img_src = cv2.imread(img_src_filename, 0);
height, width = img_src.shape

current_character = 1
characters_count = 224

bounding_width = 34
bounding_height = 41

offset_x = 0
offset_y = 0

ret, thresh = cv2.threshold(255-img_src,0,255,cv2.THRESH_BINARY|cv2.THRESH_OTSU)
num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(thresh, 8, cv2.CV_32S)

# The backgorund will present the maximum number of pixels:
bg_label = int(np.floor(np.argmax(stats) / 5.0))
stats = np.array(stats)
stats = np.delete(stats, bg_label, 0)
centroids = np.array(centroids)
centroids = np.delete(centroids, bg_label, 0)
num_labels = num_labels-1
labels[labels >= bg_label] = labels[labels >= bg_label] - 1

pairs_distances = np.zeros(num_labels * (num_labels + 1) / 2)
pairs_indices = []
max_distance = 400.0;

for i in range(num_labels):
    position_offset = i * (i + 1) / 2
    for j in xrange(i):
        pairs_distances[position_offset + j] = np.sum((centroids[i] - centroids[j])**2)
        pairs_indices.append([i, j])
        
    pairs_distances[position_offset + i] = max_distance - 1e-1
    pairs_indices.append([i, i])

pairs_sets = []
pairs_sets_dictionary = {}
nearest_pairs = np.where(pairs_distances < max_distance)
for near_pair in nearest_pairs[0]:
    pair_a, pair_b = pairs_indices[near_pair]
    pair_index = -1    
    if (pair_a in pairs_sets_dictionary):
        pair_index = pairs_sets_dictionary[pair_a]
        pairs_sets_dictionary[pair_b] = pair_index
        pairs_sets[pair_index] = pairs_sets[pair_index] | set([pair_a, pair_b])
        
    elif (pair_b in pairs_sets_dictionary):
        pair_index = pairs_sets_dictionary[pair_b]
        pairs_sets_dictionary[pair_a] = pair_index
        pairs_sets[pair_index] = pairs_sets[pair_index] | set([pair_a, pair_b])
        
    else:
        pair_index = len(pairs_sets)
        pairs_sets_dictionary[pair_a] = pair_index
        pairs_sets_dictionary[pair_b] = pair_index
        pairs_sets.append(set([pair_a, pair_b]))

text_map_img = np.zeros((1024,512,3), np.uint8)

max_width = 0
max_height = 0
characters_bounding_boxes = []
for pair_set in pairs_sets:    
    character_L = 512
    character_R = 0
    character_T = 1024
    character_B = 0
    
    center_x = 0.0
    center_y = 0.0
    elements_in_character = 0.0
    
    for pair_i in list(pair_set):
        center_x_i, center_y_i = centroids[pair_i]
        center_x = center_x + center_x_i
        center_y = center_y + center_y_i
        elements_in_character = elements_in_character + 1
        
        character_L_i = stats[pair_i, 0]
        character_R_i = stats[pair_i, 0] + stats[pair_i, 2]
        character_T_i = stats[pair_i, 1]
        character_B_i = stats[pair_i, 1] + stats[pair_i, 3]
        
        if (character_L > character_L_i):
            character_L = character_L_i
            
        if (character_T > character_T_i):
            character_T = character_T_i
            
        if (character_R < character_R_i):
            character_R = character_R_i
            
        if (character_B < character_B_i):
            character_B = character_B_i
    
    character_width = character_R - character_L
    character_height = character_B - character_T
    
    if (max_width < character_width):
        max_width = character_width
        
    if (max_height < character_height):
        max_height = character_height
    
    characters_bounding_boxes.append([pair_set, center_x / elements_in_character, center_y / elements_in_character, character_L, character_T, character_R, character_B])

text_map_img[:,:,1] = 255

text_map_img[:,:,0] = text_map_img[:,:,0] + thresh
text_map_img[:,:,1] = text_map_img[:,:,1] - thresh

text_map_img[-max_height:, -max_width:, 0] = 255
text_map_img[-max_height:, -max_width:, 1] = 0
text_map_img[-(max_height-3):-3, -(max_width-3):-3, 0] = 0
text_map_img[-(max_height-3):-3, -(max_width-3):-3, 1] = 255


unknown_char_L = 512.0 - max_width
unknown_char_T = 1024.0 - max_height
unknown_char_R = 512.0
unknown_char_B = 1024.0


rightmost_element = np.max(stats[:, 0] + stats[:, 2])
leftmost_element = np.min(stats[:, 0])

topmost_element = np.min(stats[:, 1])
bottommost_element = np.max(stats[:, 1] + stats[:, 3])

offset_x = rightmost_element / 30.0 + leftmost_element
char_width = rightmost_element / 15.0

offset_y = bottommost_element / 30.0 + topmost_element
char_height = bottommost_element / 15.0

character_id = 32

topmost_element_per_line = []
characters_boundaries = []
for i in range(15):
    topmost_element_this_line = 1024
    for j in range(15):
        cent_a_x = offset_x + char_width * j
        cent_a_y = offset_y + char_height * i
        
        min_distance_x = (char_width * 0.5) ** 2
        min_distance_y = (char_height * 0.5) ** 2
        nearest_char = -1
        nearest_char_L = -1
        nearest_char_R = -1
        nearest_char_T = -1
        nearest_char_B = -1
        nearest_center_x = -1
        nearest_center_y = -1
        
        for pair_set, character_center_x, character_center_y, character_L, character_T, character_R, character_B in characters_bounding_boxes:
            distance_x = (cent_a_x - character_center_x) ** 2
            distance_y = (cent_a_y - character_center_y) ** 2
            
            if (min_distance_x > distance_x and min_distance_y > distance_y):
                nearest_char = 1
                min_distance_x = distance_x
                min_distance_y = distance_y
                nearest_char_L = character_L
                nearest_char_R = character_R
                nearest_char_T = character_T
                nearest_char_B = character_B
                nearest_center_x = character_center_x
                nearest_center_y = character_center_y
        
        if (nearest_char >= 0): 
            nearest_was_found = 1
            if (topmost_element_this_line > nearest_char_T):
                topmost_element_this_line = nearest_char_T
            
            U_coord_1, V_coord_1 = nearest_char_L / 512.0, nearest_char_B/1024.0
            U_coord_2, V_coord_2 = nearest_char_L / 512.0, nearest_char_T/1024.0
            U_coord_3, V_coord_3 = nearest_char_R / 512.0, nearest_char_T/1024.0
            U_coord_4, V_coord_4 = nearest_char_L / 512.0, nearest_char_B/1024.0
            U_coord_5, V_coord_5 = nearest_char_R / 512.0, nearest_char_B/1024.0
            U_coord_6, V_coord_6 = nearest_char_R / 512.0, nearest_char_T/1024.0
            
        elif (i == 0 and j == 0):
            nearest_was_found = 1
            nearest_char_L = 0
            nearest_char_R = max_width - 1
            nearest_char_T = 0
            nearest_char_B = max_height - 1
            U_coord_1, V_coord_1 = 0.0, max_height/1024.0
            U_coord_2, V_coord_2 = 0.0, 0.0
            U_coord_3, V_coord_3 = max_width / 512.0, 0.0
            U_coord_4, V_coord_4 = 0.0, max_height/1024.0
            U_coord_5, V_coord_5 = max_width / 512.0, max_height/1024.0
            U_coord_6, V_coord_6 = max_width / 512.0, 0.0
            
        else:
            nearest_was_found = 0
            nearest_char_L = 0
            nearest_char_R = max_width - 1
            nearest_char_T = 0
            nearest_char_B = max_height - 1
            U_coord_1, V_coord_1 = unknown_char_L / 512.0, unknown_char_B/1024.0
            U_coord_2, V_coord_2 = unknown_char_L / 512.0, unknown_char_T/1024.0
            U_coord_3, V_coord_3 = unknown_char_R / 512.0, unknown_char_T/1024.0
            U_coord_4, V_coord_4 = unknown_char_L / 512.0, unknown_char_B/1024.0
            U_coord_5, V_coord_5 = unknown_char_R / 512.0, unknown_char_B/1024.0
            U_coord_6, V_coord_6 = unknown_char_R / 512.0, unknown_char_T/1024.0
        
        characters_boundaries.append([character_id, nearest_char_L, nearest_char_T, nearest_char_R, nearest_char_B, 0, U_coord_1, V_coord_1,U_coord_2, V_coord_2,U_coord_3, V_coord_3,U_coord_4, V_coord_4,U_coord_5, V_coord_5,U_coord_6, V_coord_6, nearest_was_found])
        character_id = character_id + 1
    
    topmost_element_per_line.append(topmost_element_this_line)


# Add the offset in y-axis to each character:
characters_boundaries[0][5] = 0
char_id = 1
i = 0
while(char_id < 224):
    if ((char_id % 15) == 0):
        i = i + 1
    
    print('{}, top {}, top in line {}, offset {}'.format(char_id, characters_boundaries[char_id][2], topmost_element_per_line[i], characters_boundaries[char_id][2] - topmost_element_per_line[i]))
    if (characters_boundaries[char_id][18] > 0):
        characters_boundaries[char_id][5] = characters_boundaries[char_id][2] - topmost_element_per_line[i]
        
    char_id = char_id + 1

plt.imshow(text_map_img)
plt.show()

positions = open('letter_positions.dat', 'w')
positions.write('CODE\tLeftmost\tTopmost\tRightmost\tBottommost\tOffset_Y\tU_COORD_1\tV_COORD_1\tU_COORD_2\tV_COORD_2\tU_COORD_3\tV_COORD_3\tU_COORD_4\tV_COORD_4\tU_COORD_5\tV_COORD_5\tU_COORD_6\tV_COORD_6\n')
for character_id, nearest_char_L, nearest_char_T, nearest_char_R, nearest_char_B, offset_character_Y, U_coord_1, V_coord_1,U_coord_2, V_coord_2,U_coord_3, V_coord_3,U_coord_4, V_coord_4,U_coord_5, V_coord_5,U_coord_6, V_coord_6, nearest_was_found in characters_boundaries:
    positions.write('{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\n'.format(character_id, nearest_char_L, nearest_char_T, nearest_char_R, nearest_char_B, offset_character_Y, U_coord_1, V_coord_1,U_coord_2, V_coord_2,U_coord_3, V_coord_3,U_coord_4, V_coord_4,U_coord_5, V_coord_5,U_coord_6, V_coord_6))

positions.close()

cv2.imwrite('texture_typography_map.ppm', text_map_img)
