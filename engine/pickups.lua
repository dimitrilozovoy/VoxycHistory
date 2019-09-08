function spawn_pickups()
    local floppyidx = 1
    local tapeidx = 1
    local playeridx = 1
    for k, v in pairs(allobjs) do
    
      if v == "voxels" then

        local shape = getshape(k)
        local voxsize = getvoxsize(shape)
        local floppy = gettexvox(shape, "floppy.png")
        local tape = gettexvox(shape, "tape.png")
        local player = gettexvox(shape, "player.png")

        log("voxsize" .. voxsize)
      
        for x = 0, voxsize, 1 do
          for y = 0, voxsize, 1 do
            for z = 0, voxsize, 1 do
              local v = getvox(shape, x, y, z)
              
              local cx, cy, cz = getpos(k)
              local onevoxelsize = chunkworldsize / voxsize
              local wx = cx - onevoxelsize * ((voxsize / 2) - x)
              local wy = cy - onevoxelsize * ((voxsize / 2) - y)
              local wz = cz - onevoxelsize * ((voxsize / 2) - z)
                
              local dsx = wx + onevoxelsize / 2
              local dsy = wy + onevoxelsize * 2
              local dsz = wz + onevoxelsize / 2
              local dsyaw = 0

              if v == floppy then
                              
                if getvox(shape, x, y, z - 1) ~= 0 and getvox(shape, x, y, z + 1) ~= 0 then
                  dsyaw = 90
                end
                
                setvox(shape, x, y, z, 0)
                
                local name = "floppy" .. math.floor(floppyidx)
                addobj(name)
                settype(name, "sprite")
                settex(name, "floppy.png")
                
                setpos(name, dsx, dsy, dsz)
                setyaw(name, dsyaw)
                setsize(name, onevoxelsize * 4, onevoxelsize * 4, onevoxelsize * 4)
                setphyssize(name, onevoxelsize, onevoxelsize, onevoxelsize)
                setfaceplayer(name, true)
                
                floppyidx = floopyidx + 1
                
              end
              
              if v == tape then
                                         
                setvox(shape, x, y, z, 0)
                
                local name = "window" .. math.floor(idx)
                addobj(name)
                settype(name, "sprite")
                settex(name, "tape.png")
                
                setpos(name, dsx, dsy, dsz)
                setyaw(name, dsyaw)
                setsize(name, onevoxelsize, onevoxelsize, onevoxelsize)
                setphyssize(name, onevoxelsize, onevoxelsize, onevoxelsize)
                setfaceplayer(name, true)
                
                tapeidx = tapeidx + 1
                
              end

              if v == player then
                                        
                setvox(shape, x, y, z, 0)
                
                local name = "pc" .. math.floor(idx)
                addobj(name)
                settype(name, "sprite")
                settex(name, "player.png")
                
                setpos(name, dsx, dsy, dsz)
                setyaw(name, dsyaw)
                setsize(name, onevoxelsize, onevoxelsize, onevoxelsize)
                setphyssize(name, onevoxelsize, onevoxelsize, onevoxelsize)
                setfaceplayer(name, true)
                
                playeridx = playeridx + 1
                
              end
            end
          end
        end
      end
    end

end

function tick_pickups()
end