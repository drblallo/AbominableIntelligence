from matplotlib import pyplot as plt
from sys import stdin
import argparse
import re

def any_match(regexes, key):
    return any(re.match(regex, key) for regex in regexes)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='show values.')
    parser.add_argument('--exclude', metavar='E', type=str, nargs="*", default="", help='Things not to show')
    args = parser.parse_args()

    lines = stdin.readlines()
    id_val = [line.split(',') for line in lines]

    lists = {}
    for l in id_val:
        id = l[0]
        val = float(l[1])
        if id not in lists:
            lists[id] = []
        lists[id].append(val)
    
    for key, list in lists.items():
        if not any_match(args.exclude, key):
            plt.plot(range(len(list)), list, label=key)
    
    plt.legend()
    plt.show()
