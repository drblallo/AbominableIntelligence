from matplotlib import pyplot as plt

def plot_lines(y_values_lists, x_values = None, labels = None):
    if x_values is None:
        x_values = range(len(y_values_lists[0]))

    if labels is None:
        labels = [str(i) for i in range(len(y_values_lists))]

    for y_values, label in zip(y_values_lists, labels):
        plt.plot(x_values, y_values, label=label)

