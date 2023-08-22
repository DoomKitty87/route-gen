var fs = require('fs');
var densities = [];
var result = fs.readFileSync('densitylistv3.txt', 'utf8');
densities = result.split("\n");
console.log(densities);
var pads = [];
var i = 0;
result = fs.readFileSync('padsv3.txt', 'utf8');
result.split("\n").forEach((line) => {
  var pad = line.split(' ');
  if (pad[0] != '') pads.push({x: pad[0], y: pad[1], z: pad[2].slice(0, -1), density: densities[i].slice(0, -1)});
  i++;
});

console.log(pads);
var outString = JSON.stringify(pads);
fs.writeFileSync('pads.json', outString);