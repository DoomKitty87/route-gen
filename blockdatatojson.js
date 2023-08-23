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

const jadecoordsx = 823;
const jadecoordsz = 202;

const sectors = [];
for (let i = 0; i < 5; i++) {
  for (let j = 0; j < 5; j++) {
    const sectortmp = [];
    const cornerx = jadecoordsx + i * -128;
    const cornerz = jadecoordsz + j * 128;
    sectortmp.push(cornerx);
    sectortmp.push(cornerz);
    sectortmp.push(cornerx - 128);
    sectortmp.push(cornerz + 128);
    sectors.push(sectortmp);
  }
}

for (let sec = 0; sec < 25; sec++) {
  if (sec == 12) continue;
  const sector = sectors[sec];
  const sectorx = sector[0];
  const sectorz = sector[1];
  const secData = [];
  secData.length = 128;
  for (let x = 0; x < 128; x++) {
    secData[x] = [];
    secData[x].length = 256;
    for (let y = 0; y < 256; y++) {
      secData[x][y] = [];
      secData[x][y].length = 128;
      for (let z = 0; z < 128; z++) {
        if (sectorx - x < 0 || sectorx - x > 621 || sectorz + z > 621) secData[x][y][z] = 0;
        else secData[x][y][z] = blockData[sectorx - x][y][sectorz + z];
      }
    }
  }
  const secDataOut = JSON.stringify(secData);
  fs.writeFileSync(`sector${sec}.json`, secDataOut);
}