import amulet
import json

level = amulet.load_level("world")
blocks = [];
for x in range(202, 824):
  for y in range(0, 256):
    for z in range(202, 824):
      block = level.get_block(x, y, z, "minecraft:overworld")
      if (block.base_name == "stained_glass" or block.base_name == "stained_glass_pane"):
        #print(level.get_block(x, y, z, "minecraft:overworld").properties['color'])
        if (block.properties['color'] == "red"):
          blocks.append([x, y, z])
          print(len(blocks))
with open('ruby.json', 'w') as f:
  json.dump(blocks, f)
level.close()