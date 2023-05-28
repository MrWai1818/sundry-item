import random
import matplotlib.pyplot as plt
def roll():
    roll = random.randint(1,6)
    return roll
def main():
    fig = plt.figure()
    ax1 = fig.add_subplot(131)
    ax2 = fig.add_subplot(132)
    ax3 = fig.add_subplot(133)
    dict_1 = dict(zip(list(range(1, 7)), [0]*6))
    #n=10
    for i in range(10): 
            pin =roll()
            for j in range(1,7):
                if (pin) == j:
                    dict_1[j] += 1
    for k, r in dict_1.items():
        print('point {} times {} rate {}'.format(k, r, float(r/10)))
        rect = ax1.bar(k,height=r,width=0.5,align='center',color='pink')
        for rec in rect:
            x=rec.get_x()
            height=rec.get_height()
            ax1.text(x+0.1,1.02*height,str(r))
    ax1.set_title("n=10")
     #n=1000
    for i in range(1000):
            pin =roll()
            for j in range(1,7):
                if (pin) == j:
                    dict_1[j] += 1
    for k, r in dict_1.items():
        print('point {} times {} rate {}'.format(k, r, float(r/1000)))
        rect = ax2.bar(k,height=r,width=0.5,align='center',color='pink')
        for rec in rect:
            x=rec.get_x()
            height=rec.get_height()
            ax2.text(x+0.1,1.02*height,str(r))
    ax2.set_title("n=1000")        
    #n=100000        
    for i in range(100000): 
            pin =roll()
            for j in range(1,7):
                if (pin) == j:
                    dict_1[j] += 1
    for k, r in dict_1.items():
        print('point {} times {} rate {}'.format(k, r, float(r/100000)))
        rect = ax3.bar(k,height=r,width=0.5,align='center',color='pink')
        for rec in rect:
            x=rec.get_x()
            height=rec.get_height()
            ax3.text(x,1.02*height,str(r))
    ax3.set_title("n=100000")
    plt.show()
if __name__ == '__main__':
    main()
