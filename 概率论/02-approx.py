import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import matplotlib.style as style
from scipy.stats import binom
from scipy.stats import poisson
fig = plt.figure()
ax1 = fig.add_subplot(131)
ax2 = fig.add_subplot(132)
ax3 = fig.add_subplot(133)
#n=10 p=0.8
ax1.plot(range(0,11), binom.pmf(range(0,11), 10, 0.8),marker='.',label='Binomial')
ax1.plot(range(0,11),poisson.pmf(range(0,11),8),'--',marker='*',label='Poisson')
#n=40 p=0.2
ax2.plot(range(0,41), binom.pmf(range(0,41), 40, 0.2),marker='.',label='Binomial')
ax2.plot(range(0,41),poisson.pmf(range(0,41),8),'--',marker='*',label='Poisson')
#n=160 p=0.05
ax3.plot(range(0,161), binom.pmf(range(0,161), 160, 0.05),marker='.',label='Binomial')
ax3.plot(range(0,161),poisson.pmf(range(0,161),8),'--',marker='*',label='Poisson')
ax1.legend()
ax2.legend()
ax3.legend()
ax1.set_title("n = 10,p = 0.8")
ax2.set_title("n = 40,p = 0.2")
ax3.set_title("n = 160,p = 0.05")
plt.show()