numexplosions = 4
armedexpl = 1

-------------------------------
-- SPAWN
-------------------------------

function spawn_explosions()

  for i = 1, numexplosions, 1 do
    local name = "expl" .. i

    addobj(name)
    settype(name, "sprite")
    settex(name, "expla1.png")
    setpos(name, 0, -1000, 0)
    setsize(name, 15, 15, 15)
    setcolor(name, 1.0, 1.0, 1.0, 1.0)
    setvisible(name, false)
    setfade(name, 2000, 3000)
    setfaceplayer(name, true)
    setobjint(name, "ignorecollisions", 1)
    setobjint(name, "frame", 1)
  end
  
end

-------------------------------
-- TICK
-------------------------------

function tick_explosions()

    -- Explosion animation
    
    for i = 1, numexplosions, 1 do
      local name = "expl" .. i
	  if getvisible(name) then
	    local frame = getobjint(name, "frame")
        if frame < 8 then
          settex(name, "expla" .. math.floor(frame) .. ".png")
          setobjint(name, "frame", frame + 1)
--[[          if frame > 4 then
            setlight(name, (8 - frame) / 3, 3, 2, 1)
          else
            setlight(name, frame / 2, 3, 2, 1)
          end]]
        else
          setvisible(name, false)
--          setlight(name, 0)
        end
      end
    end
    
end

-------------------------------
-- START EXPLOSIONS
-------------------------------

function start_explosion(x, y, z)

  local armedexplname = "expl" .. armedexpl    
  setpos(armedexplname, x, y, z)
  setsize(armedexplname, 10, 10, 10)
  setvisible(armedexplname, true)
  setobjint(armedexplname, "frame", 1)
--  playsound("explosion1", true)
    
  armedexpl = armedexpl + 1

  if armedexpl >= numexplosions then
    armedexpl = 1
  end
  
end