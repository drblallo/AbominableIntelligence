import argparse
import re
from sys import stdin
from matplotlib import pyplot as plt
from plotter import plot_lines

def any_match(regexes, key):
    return any(re.match(regex, key) for regex in regexes)

def string_to_pair(line, split_char):
    line_pair = line.split(split_char)
    return (line_pair[0], float(line_pair[1]))

def comma_separated_list_to_name_points_list(lines): 
    pairs = [string_to_pair(l, ', ') for l in lines]
    lists = {}
    for key, val in pairs:
        lists.setdefault(key, []).append(val)

    return lists 

def input_to_lines_list(lines, exclude_regex):
    lists = [(name, lines) for name, lines in 
            comma_separated_list_to_name_points_list(lines).items() 
            if not any_match(exclude_regex, name)]
    names = [i[0] for i in lists]
    lines = [i[1] for i in lists]

    return (names, lines)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='show values.')
    parser.add_argument('--exclude', 
                        metavar='E', 
                        type=str, 
                        nargs="*", 
                        default="", 
                        help='Things not to show')

    args = parser.parse_args()

    (names, points_list) = input_to_lines_list(stdin.readlines(), args.exclude)

    plot_lines(points_list, labels=names)
    plt.legend()
    plt.show()
