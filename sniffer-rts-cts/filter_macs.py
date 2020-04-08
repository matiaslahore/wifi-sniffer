
from collections import OrderedDict

"""
text_file = open("wifi_scanned.txt", "r")
lines = text_file.read().split(',\n')
lines_filtred = list(set(lines));

print lines
print len(lines)
print()
print lines_filtred
print len(lines_filtred)


text_file.close()

"""

text_file = open("wifi_scanned_filtred.txt", "r")
lines = text_file.read().split(',\n')
print lines
print len(lines)