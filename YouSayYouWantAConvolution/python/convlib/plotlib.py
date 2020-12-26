import matplotlib.pyplot as plt

def plot_convolution(impulse, x, convolved):

    figure, (axis_1, axis_2, axis_3) = plt.subplots(3, 1)

    axis_1.set_title("Impulse")
    axis_1.plot(impulse)

    axis_2.set_title("Input Function")
    axis_2.plot(x)

    axis_3.set_title("Convolved Function")
    axis_3.plot(convolved)

    plt.show()
