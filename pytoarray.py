import numpy as np
import io

world_arr = np.load("world_arr.npy")
outfile = open("blockarraydatav3.txt", "w")
outfile.write("")
outfile.close()
outfile = open("blockarraydatav3.txt", "a")

for x in range(622):
  for y in range(256):
    for z in range(622):
      outfile.write(str(world_arr[x + 10, z + 10, y]) + " ")
    outfile.write("\n")
  outfile.write("\n")
outfile.close()
