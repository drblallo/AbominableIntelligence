from matplotlib import pyplot as plt
from sys import stdin

if __name__ == "__main__":
    lines = stdin.readlines()
    plt.plot([float(x) for x in lines])
    plt.show()
