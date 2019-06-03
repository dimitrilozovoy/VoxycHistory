function spawn_computers()
  local idx = 1
  for k, v in pairs(allobjs) do
      if v == "voxels" then
--        log(k .. v)
        local shape = getshape(k)
        local voxsize = getvoxsize(shape)
        for x = 0, voxsize, 1 do
          for y = 0, voxsize, 1 do
            for z = 0, voxsize, 1 do
              local v = getvox(shape, x, y, z)
              if v == 10 then
                local name = "ufo" .. math.floor(idx)
                log(name)
                addobj(name)
                settype(name, "sprite")
                settex(name, "ufo1.png")
                
                local cx, cy, cz = getpos(k)
                local onevoxelsize = chunksize / voxsize
                local wx = cx - onevoxelsize * ((voxsize / 2) - x)
                local wy = cy - onevoxelsize * ((voxsize / 2) - y)
                local wz = cz - onevoxelsize * ((voxsize / 2) - z)
--                log(wx .. " " .. wy .. " " .. wz)
                setpos(name, wx, wy, wz)
                setsize(name, 10, 10, 10)
                
                setvox(shape, x, y, z, 0)
                
                idx = idx + 1
              end
            end
          end
        end
      end
  end
end

function tick_computers()
end