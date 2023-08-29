import json

blocks = json.load(open("ruby.json", "r"))

blockData = []
for x in range(0, 622):
  blockData.append([])
  for y in range(0, 256):
    blockData[x].append([])
    for z in range(0, 622):
      blockData[x][y].append(0)

for b in blocks:
  blockData[b[0] - 202][b[1]][b[2] - 202] = 1

pads = []

for x in range(0, 622):
  print(x)
  for y in range(0, 254):
    for z in range(0, 622):
      if (x > 460 and x < 565 and z > 460 and z < 565): continue
      if (blockData[x][y + 1][z] == 0 and blockData[x][y + 2][z] == 0):
        density = 0
        for dx in range(-1, 2):
          for dy in range(1, 6):
            for dz in range(-1, 2):
              if (x + dx >= 0 and x + dx < 622 and y + dy >= 0 and y + dy < 256 and z + dz >= 0 and z + dz < 622):
                density += blockData[x + dx][y + dy][z + dz]
        if density > 10:
          pads.append({"x": x + 202, "y": y, "z": z + 202, "density": density * 3})

with open('rubyhybrid.json', 'w') as f:
  json.dump(pads, f)