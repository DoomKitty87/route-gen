import json

blocks = json.load(open("ruby.json", "r"))

for x in range(0, 622):
  for y in range(0, 256):
    for z in range(0, 622):
      