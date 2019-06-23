import math
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt


# generate point density with scipy.stats
x, y = np.loadtxt("mcmc.dat").T
xmin, xmax, ymin, ymax = x.min(), x.max(), y.min(), y.max()
X, Y = np.mgrid[xmin:xmax:100j, ymin:ymax:100j]

positions = np.vstack([X.ravel(), Y.ravel()])
values = np.vstack([x, y])
kernel = stats.gaussian_kde(values)

Z = np.reshape(kernel(positions).T, X.shape)
dots = [[X.flatten()[i], Y.flatten()[i], Z.flatten()[i]]
        for i in range(len(X.flatten()))]

with open("mcmcdist.csv", "w") as f:
    for i in range(len(dots)):
        f.write("%f %f %f\n" % (dots[i][0], dots[i][1], dots[i][2]))
        if (i + 1) % X.shape[0] == 0:
            f.write("\n")
print(np.array(dots).shape)

# if __name__ == "__main__":
    # plt.contour(X, Y, Z/Z.max(), levels=3)
    # plt.colorbar()
    # plt.show()


# # point density by counting :)
# dist = np.zeros((100, 100))
# for i in range(len(x)):
    # c = math.floor((x[i]-x.min())/(x.max()-x.min()+1e-6)*100)
    # r = math.floor((y[i]-y.min())/(y.max()-y.min()+1e-6)*100)
    # dist[c][r] += 1
# d = []
# for i in range(100):
    # for j in range(100):
        # d.append([x.min() + i * (x.max() - x.min()) / 100,
                  # y.min() + j * (y.max() - y.min()) / 100,
                  # dist[i][j]])
