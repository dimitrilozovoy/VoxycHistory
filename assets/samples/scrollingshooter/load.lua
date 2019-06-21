runscript("explosions.lua")
runscript("projectiles.lua")
runscript("computer.lua")
runscript("player.lua")

curarea = 1
chunksize = 160
numchunks = 32

-------------------------------
-- START
-------------------------------

function start(area)

  clear()

  curarea = area
  setskybox("sky1.png")
  spawn_player()

  -- Set up world

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
  
  getallobjs()

  spawn_computers()
  spawn_explosions()
  spawn_projectiles()
  
  getallobjs()

end

start()
