import json

blocks = json.load(open("ruby.json", "r"))
outblocks = blocks
print(len(blocks))
for b in blocks:
  for d in outblocks:
    if (abs(b[0] - d[0]) <= 3 and abs(b[1] - d[1]) <= 3 and abs(b[2] - d[2]) <= 3):
      outblocks.remove(d)
      #print("Removed block at " + str(d))
print(len(blocks))
with open('rubypads.json', 'w') as f:
  json.dump(blocks, f)