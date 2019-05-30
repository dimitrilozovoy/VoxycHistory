curarea = 1
chunksize = 160
numchunks = 32

function start(area)
curarea = area
setskybox("sky1.png")

-- Create player

addobj("player")
settype("player", "sprite")
settex("player", "player.png")

setpos("player", 0.0, 10.0, -50.0)
setsize("player", 5.0, 5.0, 5.0)
setcolor("player", 1.0, 1.0, 1.0, 1.0)
sethitpts("player", 100)
setmeshcolor("player", 1, 1.0, 1.0, 1.0, 0.5)
setmovesmoothly("player", true)

setplayerobj("player")
setcontrolscheme("swipexy")
setcamerascheme("thirdpersonfixed")

setvelocity("player", 0, 0, -10)
setplayerrangex(-50, 50)
setplayerrangey(-50, 50)
setplayermovespeed(0.05)

-- Test

settexspan("wall1.png", 2, 2)
settexspan("wall2.png", 4, 4)

local vox = 1
for i = 1, numchunks, 1 do
local name = "voxels" .. math.floor(i)
newshape(name, "voxels")
setvoxels(name, math.floor(vox) .. ".vx")

addobj(name)
settype(name, "shape")
setshape(name, name)
setpos(name, 0.0, 0.0, 0.0 - chunksize * i)
setsize(name, chunksize, chunksize, chunksize)
setvisible(name, true)

vox = vox + 1
if vox > 5 then
  vox = 1
end
end

end

start()
