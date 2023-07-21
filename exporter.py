import anvil
import json

files = ["r.-1.-1.mca", "r.-1.0.mca", "r.-1.1.mca", "r.0.-1.mca", "r.0.0.mca", "r.0.1.mca", "r.1.-1.mca", "r.1.0.mca", "r.1.1.mca", "r.1.2.mca", "r.2.-1.mca", "r.2.-2.mca", "r.2.0.mca", "r.2.1.mca", "r.2.2.mca", "r.3.-1.mca", "r.3.-2.mca", "r.3.0.mca", "r.3.1.mca", "r.3.2.mca"]

blocks = {}
panes = {}

blocksexp = ""
panesexp = ""

for f in files:
    region = anvil.Region.from_file(f)
    chunk = region.get_chunk(-1, -1)
    for x in range(16):
      for y in range(256):
        for z in range(16):
          if chunk.get_block(x, y, z).id == "stained_glass":
            blocks += [x, y, z]
          elif chunk.get_block(x, y, z).id == "stained_glass_pane":
            panes += [x, y, z]

for b in blocks:
  blocksexp += f"{b[0]} {b[1]} {b[2]}\n"
  
for p in panes:
  panesexp += f"{p[0]} {p[1]} {p[2]}\n"

with open("blocks.txt", "w") as f:
  f.write(blocksexp)

with open("panes.txt", "w") as f:
  f.write(panesexp)