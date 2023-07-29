import json

route = json.load(open("route.json"))
arr = []

for i in range(len(route) - 1):
    x = route[i]["x"]
    y = route[i]["y"]
    z = route[i]["z"]
    nx = route[i+1]["x"]
    ny = route[i+1]["y"]
    nz = route[i+1]["z"]
    d = ((nx - x)*2 + (ny - y)*2 + (nz - z)*2)*(1/2)
    arr.append(d)

print(arr)