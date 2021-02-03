from sys import stdin
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='repeat input multiple times.')
    parser.add_argument('quantity', metavar='N', type=int, nargs="?", default=-1, help='Number of times to repeat')

    args = parser.parse_args()
    lines = stdin.readlines()
    if len(lines) == 0:
        print("No lines")
        exit(-1)

    if args.quantity == -1:
        while True:
            [print(line[:-1]) for line in lines]
    else:
        for i in range(args.quantity):
            [print(line[:-1]) for line in lines]

