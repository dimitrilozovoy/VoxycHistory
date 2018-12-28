function makebuilding(chunk, ctrx, ctry, sizex, sizey, height)
  -- North wall
end

function buildchunk(wx, wz, wsize, size)
  local name = "voxels" .. wx .. wz

  addobj(name)
  newshape(name, "voxels", size)
  settype(name, "shape")
  setshape(name, name)
  setpos(name, wx, 5, wz)
  setsize(name, wsize, wsize, wsize)
--  setcolor(name, 1, 1, 1, 1)

  for x = 0, size, 1 do
    for z = 0, size, 1 do
      setvox(name, x, 0, z, 3)
    end
  end

  setvoxtex(name, 1, "brick3.png")
  setvoxtex(name, 2, "brick4.png")
  setvoxtex(name, 3, "concrete1.png")
  setvoxtex(name, 4, "tile.png")
  setvoxtex(name, 5, "stone1.png")
  setvoxtex(name, 6, "stone2.png")
  setvoxtex(name, 7, "beigestone.png")
  setvoxtex(name, 8, "asphalt.png")
  setvoxtex(name, 9, "graychips.png")
  setvoxtex(name, 10, "leaves.png")
  setvoxtex(name, 11, "bluecube.png")
  
  settexspan("brick3.png", 2.0, 2.0)
  settexspan("brick4.png", 2.0, 2.0)
  settexspan("concrete1.png", 2.0, 2.0)
  settexspan("tile.png", 2.0, 2.0)
  settexspan("stone1.png", 2.0, 2.0)
  settexspan("stone2.png", 2.0, 2.0)
  settexspan("beigestone.png", 2.0, 2.0)
  settexspan("asphalt.png", 2.0, 2.0)
  settexspan("graychips.png", 2.0, 2.0)
  settexspan("leaves.png", 2.0, 2.0)
  settexspan("bluecube.png", 2.0, 2.0)

  rebuild(name)
end

local chunksize = 16 --16
local chunkwsize = 10 --10
local size = 3 --3

for x = 0, size - 1, 1 do
  for z = 0, size - 1, 1 do
    buildchunk(-(chunkwsize * size / 2) + x * chunkwsize, -(chunkwsize * size / 2) + z * chunkwsize, chunkwsize, chunksize)
  end
end
