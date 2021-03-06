from plotter import plot_lines 

def test_plot_line():
    plot_lines([[10, 20], [20, 30]])
    plot_lines([[10, 20], [20, 30]], [0, 1])
    plot_lines([[10, 20], [20, 30]], [0, 1], ["first", "second"])
