from os import listdir
from os.path import isfile, join
import sys
from shutil import copyfile


onlyfiles = [f for f in listdir('./') if isfile(join('./', f))]

first_name_code = int(onlyfiles[1][4:-4])
last_name_code = first_name_code - 6
current_ascii_code = 32

for i, filename in enumerate(onlyfiles[1:]):
    name_code = int(filename[4:-4])
    ascii_code = current_ascii_code + 1
    new_filename = 'char_' + str(ascii_code) + '.png'
    print('{} -> {}'.format(filename, new_filename))    
    #copyfile(filename, new_filename)
    last_name_code = name_code
    if (ascii_code == 128 or ascii_code == 140 or ascii_code == 156 or ascii_code==159 or ascii_code == 173):
        ascii_code = ascii_code + 1
    
    if (ascii_code == 142):
        ascii_code = ascii_code + 2
        
    current_ascii_code = ascii_code
