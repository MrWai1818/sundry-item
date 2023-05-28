import numpy as np
from scipy.stats import norm
from scipy.stats import multivariate_normal
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
fig = plt.figure(figsize=(16,12))
#p=0
x = np.linspace(-1, 2)
y = np.linspace(-1, 2)
x,y=np.meshgrid(x,y)
pos = np.empty(x.shape + (2,))
pos[:, :, 0] = x
pos[:, :, 1] = y
ax0 = fig.add_subplot(131, projection='3d')
z0 = multivariate_normal([0.3,0.5], [[0.09,0],[0,0.25]])
ax0.plot_surface(x,y,z0.pdf(pos),rstride=1, cstride=1, cmap=plt.cm.jet)
ax0.contourf(x, y, z0.pdf(pos), cmap=plt.cm.hot)
x0 = np.linspace(-1, 2)
y0 = np.linspace(-1, 2)
ax0.plot([-1]*len(y0),y0,norm.pdf(y0,0.5,0.5),label="P(Y)")
ax0.plot(x0,[2]*len(x0),norm.pdf(x0,0.3,0.3),label="P(X)")
ax0.legend()
#p=0.9
ax1 = fig.add_subplot(132, projection='3d')
z = multivariate_normal([0.3,0.5], [[0.09,0.135],[0.135,0.25]])
ax1.plot_surface(x,y,z.pdf(pos),rstride=1, cstride=1, cmap=plt.cm.jet)
ax1.contourf(x, y, z.pdf(pos), cmap=plt.cm.hot)
x = np.linspace(-1, 2)
y = np.linspace(-1, 2)
ax1.plot([-1]*len(y),y,norm.pdf(y,0.5,0.5),label="P(Y)")
ax1.plot(x,[2]*len(x),norm.pdf(x,0.3,0.3),label="P(X)")
ax1.legend()
#p=-0.9
x2 = np.linspace(-1, 2)
y2 = np.linspace(-1, 2)
x2,y2=np.meshgrid(x2,y2)
pos = np.empty(x2.shape + (2,))
pos[:, :, 0] = x2
pos[:, :, 1] = y2
ax2 = fig.add_subplot(133, projection='3d')
z2 = multivariate_normal([0.3,0.5], [[0.09,-0.135],[-0.135,0.25]])
ax2.plot_surface(x2,y2,z2.pdf(pos),rstride=1, cstride=1, cmap=plt.cm.jet)
ax2.contourf(x2, y2, z2.pdf(pos), cmap=plt.cm.hot)
x2 = np.linspace(-1, 2)
y2 = np.linspace(-1, 2)
ax2.plot([-1]*len(y2),y2,norm.pdf(y2,0.5,0.5),label="P(Y)")
ax2.plot(x2,[2]*len(x2),norm.pdf(x2,0.3,0.3),label="P(X)")
ax2.legend()

plt.show()
