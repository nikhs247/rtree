import matplotlib.animation as ani
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import pandas as pd

df = pd.read_csv('results/iot_bb.csv', delimiter=' ', header=None)

print(df[0][0], ' ', df[1][0])

fig, ax = plt.subplots()

plt.xlabel('x')
plt.ylabel('y')

patch = patches.Rectangle((0,0), 0, 0, fc='none', ec='black')

# def init():
#     ax.add_patch(patch)
#     return patch
    
def plotRTreeBB(i):
    print(i)
    patch = patches.Rectangle((0,0), 0, 0, fc='none', ec='black')
    patch.set_width(df[2][i] - df[0][i])
    patch.set_height(df[3][i] - df[1][i])
    patch.set_xy([df[0][i], df[1][i]])
    ax.add_patch(patch)
    return patch

animate = ani.FuncAnimation(fig, plotRTreeBB,  interval=1, save_count=484)
animate.save('results/rtree_bb.gif')
plt.show()