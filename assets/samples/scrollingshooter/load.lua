
-------------------------------
-- GLOBALS
-------------------------------

numblocks = 4
blocksize = 800
curblockz = -1
lastblockz = -1
curblockx = -1
lastblockx = -1
worldsize = blocksize * numblocks
startx = -worldsize / 2.0
startz = -worldsize
yoffset = -22
numcomps = 4
numprojectilespercomp = 4
compfiretimer = {1 .. numcomps}
compfiretimerdelay1 = 100
compfiretimerdelay2 = 20
compdying = {1 .. numcomps}
compdyingtimer = {1 .. numcomps}
compdyingtimerdelay = 47
compdead = {1 .. numcomps}
comparmedproj = {1 .. numcomps}
compprojspeed = 50
playerdead = 0
playerdying = 0
numplayerprojectiles = 8
playerrevivetimer = 0
playerfiretimerdelay = 20
playerfiretimer = 0
playerarmedproj = 1
playerhurttimer = 0
playerhurttimerdelay = 100
playerdying = 0
playerscore = 0
playerdyingtimer = 0
playerdyingtimerdelay = 60
playerhitpts = 5
area = 1
nextarea = 2
areas = 4
areatexttimerdelay = 30
areatexttimer = areatexttimerdelay
areasize = worldsize * 2
--areasize = 300
areaswitchtimerdelay = 16
areaswitchtimer = -1
reviveplayeronareaswitch = 0
numtrees = 0

-------------------------------
-- TERRAIN
-------------------------------

function buildterrain(number)
  if number == 1 then
    newshape("terrain", "terrain", 8, 5, 8)

    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      addobj(name)
      settype(name, "shape")
      setshape(name, "terrain")
      setpos(name, 0, yoffset, startz + (z - 1) * blocksize + blocksize / 2.0)
	  setsize(name, blocksize, 50.0, (blocksize) * 1.05)
      setcolor(name, 1.0, 1.0, 1.0, 1.0)
      settex(name, "grass1.png")
    end
  end
  
  if number == 2 then
    newshape("terrain", "terrain", 8, 5, 8)

    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      addobj(name)
      settype(name, "shape")
      setshape(name, "terrain")
      setpos(name, 0, yoffset, startz + (z - 1) * blocksize + blocksize / 2.0)
	  setsize(name, blocksize, 50.0, (blocksize) * 1.05)
      setcolor(name, 1.0, 1.0, 1.0, 1.0)
      settex(name, "rock.png")
    end
  end
  
  if number == 3 then
    newshape("terrain", "terrain", 8, 5, 8)

    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      addobj(name)
      settype(name, "shape")
      setshape(name, "terrain")
      setpos(name, 0, yoffset, startz + (z - 1) * blocksize + blocksize / 2.0)
	  setsize(name, blocksize, 50.0, (blocksize) * 1.05)
      setcolor(name, 1.0, 1.0, 1.0, 1.0)
      settex(name, "graychips.png")
    end
  end

  if number == 4 then
    newshape("terrain", "terrain", 8, 5, 8)

    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      addobj(name)
      settype(name, "shape")
      setshape(name, "terrain")
      setpos(name, 0, yoffset, startz + (z - 1) * blocksize + blocksize / 2.0)
	  setsize(name, blocksize, 50.0, (blocksize) * 1.05)
      setcolor(name, 1.0, 1.0, 1.0, 1.0)
      settex(name, "graychips.png")
    end
  end

--      log(name .. " z " .. startz + z * blocksize + blocksize / 2.0)
end

function clearterrain(number)
  if number == 1 then
    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      remobj(name)
    end
    delshape("terrain")
  end
  if number == 2 then
    for z = 1, numblocks, 1 do        
      local name = "mountains" .. z
      remobj(name)
    end
    delshape("terrain")
  end

end

-------------------------------
-- AREAS
-------------------------------

function buildarea(number)
  buildterrain(number)

  do return end

  local townsizex = 310
  local townsizez = 800
  local numbldgsx = 12
  local numbldgsz = 5
  local bldchance = 6
  local treeidx = 1
 
  if number == 1 then
    ---------------
    -- AREA 1
    ---------------
    
    -- Town 1

    newshape("building1", "voxels", 128, 0)
    loadvox("building1", "building1.vx")

    local townctrx = -50
    local townctrz = -2800
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building1_" .. bz .. bx)
          settype("building1_" .. bz .. bx, "shape")
          setshape("building1_" .. bz .. bx, "building1")
          setpos("building1_" .. bz .. bx, x, y, z)
          setsize("building1_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains1", "building1_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 2

    newshape("building2", "voxels", 128, 0)
    loadvox("building2", "building1.vx")

    local townctrx = -50
    local townctrz = -2000
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building2_" .. bz .. bx)
          settype("building2_" .. bz .. bx, "shape")
          setshape("building2_" .. bz .. bx, "building2")
          setpos("building2_" .. bz .. bx, x, y, z)
          setsize("building2_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains2", "building2_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end
    
    -- Town 3

    newshape("building3", "voxels", 128, 0)
    loadvox("building3", "building1.vx")

    local townctrx = -50
    local townctrz = -1200
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building3_" .. bz .. bx)
          settype("building3_" .. bz .. bx, "shape")
          setshape("building3_" .. bz .. bx, "building3")
          setpos("building3_" .. bz .. bx, x, y, z)
          setsize("building3_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains3", "building3_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 4

    newshape("building4", "voxels", 128, 0)
    loadvox("building4", "building1.vx")

    local townctrx = -50
    local townctrz = -400
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building4_" .. bz .. bx)
          settype("building4_" .. bz .. bx, "shape")
          setshape("building4_" .. bz .. bx, "building4")
          setpos("building4_" .. bz .. bx, x, y, z)
          setsize("building4_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains4", "building4_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

  end
  
  if number == 2 then
    ---------------
    -- AREA 2
    ---------------
    
    -- Town 1

    newshape("building1", "voxels", 128, 0)
    loadvox("building1", "building1.vx")

    local townctrx = -50
    local townctrz = -2800
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 30

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -8
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building1_" .. bz .. bx)
          settype("building1_" .. bz .. bx, "shape")
          setshape("building1_" .. bz .. bx, "building1")
          setpos("building1_" .. bz .. bx, x, y, z)
          setsize("building1_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains1", "building1_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 2

    newshape("building2", "voxels", 128, 0)
    loadvox("building2", "building1.vx")

    local townctrx = -50
    local townctrz = -2000
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building2_" .. bz .. bx)
          settype("building2_" .. bz .. bx, "shape")
          setshape("building2_" .. bz .. bx, "building2")
          setpos("building2_" .. bz .. bx, x, y, z)
          setsize("building2_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains2", "building2_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end
    
    -- Town 3

    newshape("building3", "voxels", 128, 0)
    loadvox("building3", "building1.vx")

    local townctrx = -50
    local townctrz = -1200
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 30

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -8
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building3_" .. bz .. bx)
          settype("building3_" .. bz .. bx, "shape")
          setshape("building3_" .. bz .. bx, "building3")
          setpos("building3_" .. bz .. bx, x, y, z)
          setsize("building3_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains3", "building3_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 4

    newshape("building4", "voxels", 128, 0)
    loadvox("building4", "building1.vx")

    local townctrx = -50
    local townctrz = -400
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building4_" .. bz .. bx)
          settype("building4_" .. bz .. bx, "shape")
          setshape("building4_" .. bz .. bx, "building4")
          setpos("building4_" .. bz .. bx, x, y, z)
          setsize("building4_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains4", "building4_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

  end
  
  if number == 3 then
    ---------------
    -- AREA 3
    ---------------
    
    -- Town 1

    newshape("building1", "voxels", 128, 0)
    loadvox("building1", "building1.vx")

    local townctrx = -50
    local townctrz = -2800
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building1_" .. bz .. bx)
          settype("building1_" .. bz .. bx, "shape")
          setshape("building1_" .. bz .. bx, "building1")
          setpos("building1_" .. bz .. bx, x, y, z)
          setsize("building1_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains1", "building1_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 2

    newshape("building2", "voxels", 128, 0)
    loadvox("building2", "building1.vx")

    local townctrx = -50
    local townctrz = -2000
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building2_" .. bz .. bx)
          settype("building2_" .. bz .. bx, "shape")
          setshape("building2_" .. bz .. bx, "building2")
          setpos("building2_" .. bz .. bx, x, y, z)
          setsize("building2_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains2", "building2_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end
    
    -- Town 3

    newshape("building3", "voxels", 128, 0)
    loadvox("building3", "building1.vx")

    local townctrx = -50
    local townctrz = -1200
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building3_" .. bz .. bx)
          settype("building3_" .. bz .. bx, "shape")
          setshape("building3_" .. bz .. bx, "building3")
          setpos("building3_" .. bz .. bx, x, y, z)
          setsize("building3_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains3", "building3_" .. bz .. bx)
        else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end

    -- Town 4

    newshape("building4", "voxels", 128, 0)
    loadvox("building4", "building1.vx")

    local townctrx = -50
    local townctrz = -400
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building4_" .. bz .. bx)
          settype("building4_" .. bz .. bx, "shape")
          setshape("building4_" .. bz .. bx, "building4")
          setpos("building4_" .. bz .. bx, x, y, z)
          setsize("building4_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains4", "building4_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end
  end
  
  if number == 4 then
    ---------------
    -- AREA 4
    ---------------
    
    -- Town 1

    newshape("building1", "voxels", 128, 0)
    loadvox("building1", "building1.vx")

    local townctrx = -50
    local townctrz = -2800
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building1_" .. bz .. bx)
          settype("building1_" .. bz .. bx, "shape")
          setshape("building1_" .. bz .. bx, "building1")
          setpos("building1_" .. bz .. bx, x, y, z)
          setsize("building1_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains1", "building1_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
       end
      end
    end

    -- Town 2

    newshape("building2", "voxels", 128, 0)
    loadvox("building2", "building1.vx")

    local townctrx = -50
    local townctrz = -2000
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building2_" .. bz .. bx)
          settype("building2_" .. bz .. bx, "shape")
          setshape("building2_" .. bz .. bx, "building2")
          setpos("building2_" .. bz .. bx, x, y, z)
          setsize("building2_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains2", "building2_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
        end
      end
    end
    
    -- Town 3

    newshape("building3", "voxels", 128, 0)
    loadvox("building3", "building1.vx")

    local townctrx = -50
    local townctrz = -1200
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -5
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building3_" .. bz .. bx)
          settype("building3_" .. bz .. bx, "shape")
          setshape("building3_" .. bz .. bx, "building3")
          setpos("building3_" .. bz .. bx, x, y, z)
          setsize("building3_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains3", "building3_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
       end
      end
    end

    -- Town 4

    newshape("building4", "voxels", 128, 0)
    loadvox("building4", "building1.vx")

    local townctrx = -50
    local townctrz = -400
    local lotsizex = townsizex / numbldgsx
    local lotsizez = townsizez / numbldgsz
    local bsize = 40

    for bz = 1, numbldgsz, 1 do
      for bx = 1, numbldgsx, 1 do
        if rand(0, 10) > bldchance then
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5)
          local y = -3
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5)

          addobj("building4_" .. bz .. bx)
          settype("building4_" .. bz .. bx, "shape")
          setshape("building4_" .. bz .. bx, "building4")
          setpos("building4_" .. bz .. bx, x, y, z)
          setsize("building4_" .. bz .. bx, bsize, bsize, bsize)

          batch("mountains4", "building4_" .. bz .. bx)
       else
          local name = "tree" .. treeidx
          local x = townctrx - (townsizex / 2) + (bx * lotsizex) - (lotsizex * 0.5) + rand(-15, 15)
          local y = -15
          local z = townctrz - (townsizez / 2) + (bz * lotsizez) - (lotsizez * 0.5) + rand(-15, 15)
          
          addobj(name)
          settype(name, "sprite")
          settex(name, "tree1.png")
          setpos(name, x, y, z)
          setsize(name, 10, 10, 10)
          setcolor(name, 1.0, 1.0, 1.0, 1.0)
	  setvisible(name, true)
	  setfaceplayer(name)
          numtrees = treeidx
          treeidx = treeidx + 1
       end
      end
    end
  end
end

function cleararea(number)
  clearterrain(number)
  
  local numbldgsx = 14
  local numbldgsz = 5

  for bz = 1, numbldgsz, 1 do
    for bx = 1, numbldgsx, 1 do
      remobj("building1_" .. bz .. bx)
      remobj("building2_" .. bz .. bx)
      remobj("building3_" .. bz .. bx)
      remobj("building4_" .. bz .. bx)
    end
  end
  
  delshape("building1")
  delshape("building2")
  delshape("building3")
  delshape("building4")
  
  -- Delete trees
  for t = 1, numtrees, 1 do
    remobj("tree" .. t)
  end
  
  numtrees = 0
end

-------------------------------
-- COMPUTER
-------------------------------

function makecomputer(idx, type)
  addobj("computer" .. idx)
  settype("computer" .. idx, "model")
  setmodel("computer" .. idx, "sphere.obj")
  setpos("computer" .. idx, rand(-200, 200), 0, -300 - 500 * idx)
  setsize("computer" .. idx, 10, 10, 10)
  setcolor("computer" .. idx, 1, 1, 1, 1)
  settex("computer" .. idx, "bluecube.png")
  setfade("computer" .. idx, 2000, 3000)
  setobjint("computer" .. idx, "type", 1)
  
  comparmedproj[idx] = 1
  compfiretimer[idx] = 100
  compdead[idx] = 0
  compdying[idx] = 0
  compdead[idx] = 0

  -- Projectiles
  for i = 1, numprojectilespercomp, 1 do
	  local name = "computer" .. idx .. "proj" .. i
	  addobj(name)
	  settype(name, "sprite")
	  settex(name, "ball.png")
	  setpos(name, 0, 0, -1000)
	  setsize(name, 5, 5, 5)
     	  setcolor(name, 1.0, 1.4, 3.0, 1.0)
	  setvisible(name, false)
	  setfaceplayer(name)
      setfade(name, 2000, 3000)
  end
end

-------------------------------
-- LOAD MAIN
-------------------------------

-- Skybox, etc

setskybox("skybox.png")

addtext("score", "score 0", -0.4, 0.6, 0.1, "")

addtext("area", "area " .. area, 0.0, 0.3, 0.1, "")

buildarea(area)

local texspan = 3

settexspan("brick3.png", texspan, texspan)
settexspan("brick4.png", texspan, texspan)
settexspan("tile.png", texspan, texspan)
settexspan("concrete1.png", texspan, texspan)
settexspan("beigestone.png", 6, 6)
settexspan("asphalt.png", texspan, texspan)
settexspan("rock.png", texspan, texspan)
settexspan("subwaytile.png", texspan, texspan)
settexspan("stone1.png", texspan, texspan)
settexspan("stone2.png", texspan, texspan)

-- Create player

addobj("player")
settype("player", "model")
setmodel("player", "sphere.obj")
setpos("player", 0.0, 10.0, -50.0)
setsize("player", 5.0, 5.0, 5.0)
setcolor("player", 1.0, 1.0, 1.0, 1.0)
settex("player", "bluecube.png")
sethitpts("player", playerhitpts)
setmeshcolor("player", 1, 1.0, 1.0, 1.0, 0.5)
setmovesmoothly("player", true)

setplayerobj("player")
setcontrolscheme("scrollingshooterxy")
setcamerascheme("thirdperson")
setcontrolsvisible(true)

setvelocity("player", 0, 0, -10)
setplayerrangex(-200, 100)
setplayerrangey(-20, 50)

-- Create player projectiles

for i = 1, numplayerprojectiles, 1 do
  local name = "playerproj" .. i

  addobj(name)
  settype(name, "sprite")
  settex(name, "ball.png")
  setpos(name, 0, 0, -1000)
  setsize(name, 5, 5, 5)
  setcolor(name, 1.0, 1.4, 3.0, 1.0)
  setvisible(name, false)
  setfade(name, 2000, 3000)
end

-- Create computers

for i = 1, numcomps, 1 do
  makecomputer(i, i)
end

playtrack("galacticjourney.ogg", false)

-- Fade in

setglobalcolor(0, 0, 0, 0)
fadeglobalcolor(1, 1, 1, 1, areaswitchtimerdelay)


