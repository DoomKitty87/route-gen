import numpy

arr = numpy.load('world_arr.npy')
import sys
numpy.set_printoptions(threshold=sys.maxsize)
print(arr)