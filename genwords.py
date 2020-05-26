#!/usr/bin/env python3
"""
 * generate a random document of words
 * @file genwords.py
 * @author Pavan Dayal
"""

import random

# get list of words
word_file = open("/usr/share/dict/words")
words = set()
count = 0
for line in word_file:
    word = line.strip()
    if len(word) > 2:
        count += 1
        words.add(word)
words = list(words)

# how many random words to output
num_lines = 50
line_word_range = (10,60)

for l in range(num_lines):
    num_w = random.randint(*line_word_range)
    s = list();
    for w in range(num_w):
        w_index = random.randint(0,count-1)
        s.append(words[w_index])
    print((" ").join(s))

