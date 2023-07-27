import anvil

files = ["r.0.0.mca", "r.0.1.mca", "r.1.0.mca", "r.1.1.mca", "r.0.0.mca1", "r.0.1.mca1", "r.1.0.mca1", "r.1.1.mca1", "r.0.0.mca2", "r.0.1.mca2", "r.1.0.mca2", "r.1.1.mca2"]

blocksexp = ""

blocks = []

with open("blocksv2.txt", "w") as f:
  f.write("")

regions = []
regioncoords = []
for i in range(int(len(files) / 4)):
  for j in range(4):
    regions.append(anvil.Region.from_file(files[i * 4 + j]))
for i in range(4):
  regioncoords.append(files[i].split("."))
for i in range(4):
  for cx in range(32):
    print(len(blocks))
    for cz in range(32):
      chunks = []
      xchunk = (cx + int(regioncoords[i][1]) * 32)
      zchunk = (cz + int(regioncoords[i][2]) * 32)
      for j in range(int(len(files) / 4)):
        chunks.append(regions[j * 4 + i].get_chunk(xchunk, zchunk))
      for x in range(16):
        for y in range(256):
          for z in range(16):
            if ((x + xchunk * 16 > 460 and x + xchunk * 16 < 565) and (z + zchunk * 16 > 460 and z + zchunk * 16 < 565)): continue
            blocktypes = []
            # print(chunk.get_block(x, y, z).id)
            for chunk in chunks:
              blocktypes.append(chunk.get_block(x, y, z).id)
            #if (blocktypes.count(95) > 0): print(float(blocktypes.count(95)) / len(blocktypes))
            if (float(blocktypes.count(95)) / len(blocktypes) > 0.5): blocks.append(3)
            elif (float(blocktypes.count(160)) / len(blocktypes) > 0.5): blocks.append(2)
            else: blocks.append(0)
for n in blocks:
  blocksexp += f"{n}\n"
with open("blocksv2.txt", "w") as f:
  print(str(blocksexp.count("\n")) + " blocks")
  f.write(blocksexp)