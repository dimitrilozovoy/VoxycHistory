numcomputers = 0

-------------------------------
-- SPAWN
-------------------------------

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
                local name = "computer" .. math.floor(idx)
--                log(name)
                addobj(name)
                settype(name, "sprite")
                settex(name, "ufo1.png")
                setobjstr(name, "action", "idle")
                setobjint(name, "firetimer", 0)
                sethitpts(name, 10)
                
                local cx, cy, cz = getpos(k)
                local onevoxelsize = chunksize / voxsize
                local wx = cx - onevoxelsize * ((voxsize / 2) - x)
                local wy = cy - onevoxelsize * ((voxsize / 2) - y)
                local wz = cz - onevoxelsize * ((voxsize / 2) - z)
--                log(wx .. " " .. wy .. " " .. wz)
                setpos(name, wx, wy, wz)
                setsize(name, 15, 15, 15)
                setphyssize(name, 15, 15, 15)
                
                setvox(shape, x, y, z, 0)
                
                idx = idx + 1
              end
            end
          end
        end
      end
  end
  
  numcomputers = idx
  
end

-------------------------------
-- TICK
-------------------------------

function tick_computers()

  local px, py, pz = getpos("player")

  for i = 1, numcomputers, 1 do
    local name = "computer" .. math.floor(i)
    
    if (getvisible(name)) then
    
    local action = getobjstr(name, "action")
    local cx, cy, cz = getpos(name)
    
    if action == "idle" then
      if distance(name, "player") < 1000 then
        setobjstr(name, "action", "formation")
      end
    end
    
    if action == "formation" then
      setorient(name, 0, 0, 0)
      moveforward(name, -8)
      
      if distance(name, "player") < 500 then
        if math.random() < 0.05 then
          if cy > px then
            setobjfloat(name, "vely", -5)
          else
            setobjfloat(name, "vely", 5)
          end
          setobjstr(name, "action", "attack")
          setwaypts(name, "spiral.ms", 40, 3, 0, 0, -10)
          setmovebywaypts(name, true)
          
--          setvelocity("player", 0, 0, 0)
        end
      end
    end
    
    if action == "attack" then
    
      local firetimer = getobjint(name, "firetimer")
      
      if firetimer > 0 then
        firetimer = firetimer - 1
      end
      
      if firetimer == 0 then
        launch_projectile("redproj.png", cx, cy, cz, getpitchto(name, "player"), getyawto(name, "player"), 0, 20)
        firetimer = 20
      end
      
      setobjint(name, "firetimer", firetimer)
    
--      setorient(name, 0, 0, 0)
--      moveforward(name, 8)
--      moveup(name, getobjfloat(name, "vely"))
    end
    
    if gethitpts(name) == 0 then
      setvisible(name, false)
      start_explosion(cx, cy, cz)
    end
    
    end
  end
  
end