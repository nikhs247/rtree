import matplotlib.animation as ani
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import pandas as pd

df = pd.read_csv('results/iot_bb.csv', delimiter=' ', header=None)
df_xy = pd.read_csv('data/iot_devices.csv', delimiter=',', header=None)

fig, ax = plt.subplots()

plt.xlabel('x')
plt.ylabel('y')
plt.scatter(df_xy[1], df_xy[2], s=1)

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

animate = ani.FuncAnimation(fig, plotRTreeBB, save_count=484, interval=1000)
animate.save('results/rtree_bb.gif')