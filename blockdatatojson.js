var fs = require('fs');
var result;
result = fs.readFileSync('blockarraydatav3.txt', 'utf8');
var values = "";
values = result.replace(/\s/g,'');
var blockData = [];
console.log(values.length);
console.log(values.slice(0, 100));
var i = 0;
blockData.length = 622;
for (var x = 0; x < 622; x++) {
  blockData[x] = [];
  blockData[x].length = 256;
  for (var y = 0; y < 256; y++) {
    blockData[x][y] = [];
    blockData[x][y].length = 622;
    for (var z = 0; z < 622; z++) {
      blockData[x][y][z] = values[i];
      i++;
    }
  }
}
var blockDataOut = JSON.stringify(blockData);
fs.writeFileSync('blockarraydata.json', blockDataOut);