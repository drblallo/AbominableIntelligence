from matplotlib import pyplot as plt
from sys import stdin

if __name__ == "__main__":
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
        plt.plot(range(len(list)), list, label=key)
    
    plt.legend()
    plt.show()
