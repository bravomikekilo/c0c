#! /usr/bin/env python3

import os
import shutil
import re
import argparse

def write_to(filename, record):
    dirname = os.path.dirname(filename)
    if not os.path.isdir(dirname) and dirname != '':
        os.makedirs(dirname)
    with open(filename, 'w') as f:
        print('write split to', filename)
        f.writelines(record)


parser = argparse.ArgumentParser(description='name')

parser.add_argument('input_file', type=str)
parser.add_argument('output_dir', type=str)
parser.add_argument('-f', '--filter', type=str, default='.*')

args = parser.parse_args()

input_file = args.input_file

output_dir = args.output_dir

filter_reg = re.compile(args.filter)

if os.path.isdir(output_dir):
    shutil.rmtree(output_dir)
os.makedirs(output_dir)


with open(input_file, 'r') as in_file:
    title = None
    record = []
    for line in in_file:
        if line.startswith('##$$'):
            print('get segment', line)
            _, name = line.split()

            if title and filter_reg.match(title):
                write_to(os.path.join(output_dir, title), record)

            record.clear()
            title = name
        else:
            record.append(line)

    if title and filter_reg.match(title):
        write_to(os.path.join(output_dir, title), record)
        record.clear()




