numprojectiles = 12
armedproj = 1

-------------------------------
-- SPAWN
-------------------------------

function spawn_projectiles()

  for i = 1, numprojectiles, 1 do
    local name = "proj" .. i

    addobj(name)
    settype(name, "sprite")
    settex(name, "greenproj.png")
    setpos(name, 0, -1000, -1000)
    setsize(name, 3, 3, 3)
    setphyssize(name, 3, 3, 50)
    setcolor(name, 1.0, 1.0, 1.0, 1.0)
    setvisible(name, false)
    setfade(name, 2000, 3000)
    setfaceplayer(name, true)
  end
  
end

-------------------------------
-- TICK
-------------------------------

function tick_projectiles()

  -- Process projectiles

  for i = 1, numprojectiles, 1 do
    local name = "proj" .. i

    local stuck = getobjint(name, "stuck")
    local stuckon = getobjstr(name, "stuckon")
    
    if stuck == 1 and getvisible(name) then
--      log("proj hit " .. stuckon)
      setvisible(name, false)
--      setlight(name, 0, 0, 0, 0)
--      start_explosion(px, py, pz)
      sethitpts(stuckon, 0)
    else
      moveforward(name, 40)
    end
    
    local prx, pry, prz = getpos(name)
    local px, py, pz = getpos("player")
    
    if prz < pz - 600 or prz > prz + 100 then
      setvisible(name, false)
    end

  end
end

-------------------------------
-- LAUNCH
-------------------------------

function launch_projectile(sprite, x, y, z, pitch, yaw, roll, offset)

--  log("launch_projectile " .. x .. " " .. y .. " " .. z)

  local armedprojname = "proj" .. armedproj      
  
  setpos(armedprojname, x, y, z)      
  setorient(armedprojname, pitch, yaw, roll) 
  
  setvisible(armedprojname, true)
  moveforward(armedprojname, offset)
  
  setobjint(armedprojname, "ignorecollisions", 0)
  settype(armedprojname, "sprite")
  settex(armedprojname, sprite)
  setfaceplayer(armedprojname, true)
  
--  setphyssize(armedprojname, 1, 1, 1)
--  setlight(armedprojname, 30, 30, 1.5 , 1)
    
--  playsound("fireballlaunch", true)

  armedproj = armedproj + 1

  if armedproj >= numprojectiles then
    armedproj = 1
  end
  
end