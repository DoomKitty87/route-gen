import anvil
import json

files = ["r.0.0.mca2", "r.0.1.mca2", "r.1.0.mca2", "r.1.1.mca2"]

blocks = []

with open("mithril.json", "w") as f:
  f.write("")

for f in files:
    region = anvil.Region.from_file(f)
    regioncoords = f.split(".")
    for cx in range(32):
      print(len(blocks))
      for cz in range(32):
        xchunk = cx + int(regioncoords[1]) * 32
        zchunk = cz + int(regioncoords[2]) * 32
        chunk = region.get_chunk(xchunk, zchunk)
        if chunk == anvil.EmptyChunk(xchunk, zchunk): continue
        for x in range(16):
          for y in range(256):
            for z in range(16):
              # print(chunk.get_block(x, y, z).id)
              if ((x + xchunk * 16 > 460 and x + xchunk * 16 < 565) and (z + zchunk * 16 > 460 and z + zchunk * 16 < 565)): continue
              if chunk.get_block(x, y, z).id == 95 or chunk.get_block(x, y, z).id == 160:
                blocks.append([x + xchunk * 16, y, z + zchunk * 16])

#Changed from per-cycle due to the issue of parsing an indeterminate amount of json objects.
#Could also just save it every cycle, or every n cycles.
with open("mithril.json", "a") as f:
  print(str(len(blocks)) + " blocks")
  data = json.dumps(blocks)
  f.write(data)