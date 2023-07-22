import anvil
import json

files = ["r.0.0.mca", "r.0.1.mca", "r.1.0.mca", "r.1.1.mca"]

blocksexp = ""
panesexp = ""

blocks = []
panes = []

for f in files:
    region = anvil.Region.from_file(f)
    regioncoords = f.split(".")
    for cx in range(32):
      for cz in range(32):
        xchunk = cx + int(regioncoords[1])
        zchunk = cz + int(regioncoords[2])
        chunk = region.get_chunk(xchunk, zchunk)
        if chunk == anvil.EmptyChunk(xchunk, zchunk): continue
        for x in range(16):
          for y in range(256):
            for z in range(16):
              # print(chunk.get_block(x, y, z).id)
              if chunk.get_block(x, y, z).id == 95:
                blocks.append([x + xchunk * 16, y, z + zchunk * 16])
              elif chunk.get_block(x, y, z).id == 160:
                panes.append([x + xchunk * 16, y, z + zchunk * 16])

for b in blocks:
  blocksexp += f"{b[0]} {b[1]} {b[2]}\n"
  
for p in panes:
  panesexp += f"{p[0]} {p[1]} {p[2]}\n"
