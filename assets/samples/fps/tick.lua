
-------------------------------
-- PLAYER
-------------------------------

function playerthink()

  --
  -- Weapon
  --

  local px, py, pz = getendoftickpos("player")
  setpos("weapon", px, py - 0.25, pz)
  local ppitch, pyaw, proll = getplayerorient()

--  setorient("weapon", 0, pyaw - 90, 0)
--  moveforward("weapon", 0.03)

  setorient("weapon", 0, pyaw, 0)
  moveforward("weapon", 1)

  setpos("weaponfire", px, py - 0.1, pz)
  setorient("weaponfire", 0, pyaw, 0)
  moveforward("weaponfire", 1)

  --
  -- Attack
  --

  if playerattacktimer > 0 then
    playerattacktimer = playerattacktimer - 1
  end

  if playerattacktimer == 13 then
   setvisible("weaponfire", false)
  end

  if playerattacktimer == 0 and (getmousebtn(0) == 1 or getbtn(0) == 1 or getaxis(5) > 0) then
    local target = collray("player")
    if target ~= "" then
      local hitpts = gethitpts(target)
      hitpts = hitpts - 30
      sethitpts(target, hitpts)
      setvisible("weaponfire", true)
      playsound("shotgun", true)
      playerattacktimer = 15
    end
  end

  --
  -- Text
  --

  -- Health
  local hitpts = gethitpts("player")
  if hitpts < 0 then
    hitpts = 0
  end
  settext("health", math.floor(hitpts))

  -- Area
  if areatexttimer > 0 then
    areatexttimer = areatexttimer - 1
  end

  if areatexttimer == 0 then
    settextvisible("area", false)
  end 

end

-------------------------------
-- COMPUTER
-------------------------------

function computerthink()
  for idx = 1, numcomputers, 1 do
    
    local name = "computer" .. idx

    if getvisible(name) then

      local hitpts = gethitpts(name)

      if hitpts > 0 and rand(1, 50) <= 2 then
        setobjstr(name, "state", "attack")
        setobjint(name, "frame", 9)
        setobjint(name, "frametimer", 0)
      end

      --
      -- Attack
      --

      if getobjstr(name, "state") == "attack" then

--        if randint(1, 100) == 1 then
--          playsound("buffhorncall", 1)
--        end
          
        -- Rotate towards player

        local yaw = getyaw(name)

        local yawtoplayer = getyawto(name, "player")
        setyaw(name, rotateangletowards(getyaw(name), yawtoplayer, 15))

        -- Animate

        local frame = getobjint(name, "frame")
        local frametimer = getobjint(name, "frametimer")

        if frametimer > 0 then
          frametimer = frametimer - 1
        end

        if frametimer == 0 then

          if frame > 11 then
            frame = 9
          end

          local m = ""

          if frame == 9 then
            m = "buffhorn_000090.obj"
          end
          if frame == 10 then
            m = "buffhorn_000100.obj"
          end
          if frame == 11 then
            m = "buffhorn_000110.obj"

            if compareyaw(yaw, yawtoplayer, 10) then

              -- Fire

              local armedprojname = "proj" .. armedproj      
              local cx, cy, cz = getpos(name)
              setpos(armedprojname, cx, cy, cz)      
    
              local acy = getyaw(name)
              setyaw(armedprojname, acy) 
              setvisible(armedprojname, true)
              moveforward(armedprojname, 2)
              setobjint(armedprojname, "ignorecollisions", 0)
              setlight(armedprojname, 3, 3, 1.5 , 1)
    
              playsound("fireballlaunch", true)

              armedproj = armedproj + 1

              if armedproj >= numprojectiles then
                armedproj = 1
              end
            end

            -- Switch back to move state

            setobjstr(name, "state", "move")
            setobjint(name, "frame", 1)
            setobjint(name, "frametimer", 0)
          end

          setmodel(name, m)

          frametimer = 10
         
          -- Change frame
          frame = frame + 1
          
        end    

        setobjint(name, "frame", frame)
        setobjint(name, "frametimer", frametimer)

      end

      --
      -- Move
      --

      if getobjstr(name, "state") == "move" then

        if randint(1, 100) == 1 then
          playsound("buffhornidle", true)
        end

        local cx, cy, cz = getpos(name)
        local destx = getobjint(name, "destx")
        local destz = getobjint(name, "destz")

        local yaw = getyaw(name)

        if getobjint(name, "stuck") == 1 or (destx == 0 or desty == 0) or (math.abs(destx - cx) < 1 and math.abs(destz - cz) < 1) then

             -- Find new destination

             local px, py, pz = getplayerpos()
       
             destx = cx + rand(-10, 10)
             destz = cz + rand(-10, 10)

             setobjint(name, "destx", destx)
             setobjint(name, "destz", destz)
        
        end

        -- Turn towards our destination and move

        local yawtodest = getyawtopoint(name, destx, destz)

        if not compareyaw(yaw, yawtodest, 15) then
          setyaw(name, rotateangletowards(getyaw(name), yawtodest, 10))
          moveforward(name, 0.1)
        else
          moveforward(name, 0.1)
        end

        -- Animate

        local frame = getobjint(name, "frame")
        local frametimer = getobjint(name, "frametimer")

        if frametimer > 0 then
          frametimer = frametimer - 1
        end

        if frametimer == 0 then

          if frame > 8 then
            frame = 1
          end

          local m = ""

          if frame == 1 then
            m = "buffhorn_000010.obj"
          end
          if frame == 2 then
            m = "buffhorn_000020.obj"
          end
          if frame == 3 then
            m = "buffhorn_000030.obj"
          end
          if frame == 4 then
            m = "buffhorn_000040.obj"
          end
          if frame == 5 then
            m = "buffhorn_000050.obj"
          end
          if frame == 6 then
            m = "buffhorn_000060.obj"
          end
          if frame == 7 then
            m = "buffhorn_000070.obj"
          end
          if frame == 8 then
            m = "buffhorn_000080.obj"
          end

          setmodel(name, m)

          frametimer = 2
         
          -- Change frame
          frame = frame + 1
          
        end    

        setobjint(name, "frame", frame)
        setobjint(name, "frametimer", frametimer)

      end

      --
      -- Death
      --

      local hitpts = gethitpts(name)
      if getobjstr(name, "state") ~= "death" and getobjstr(name, "state") ~= "dead" and hitpts <= 0 then
        setobjstr(name, "state", "death")
        setobjint(name, "frame", 12)
        setobjint(name, "frametimer", 0)
      end

      if getobjstr(name, "state") == "death" then

        local frame = getobjint(name, "frame")
        local frametimer = getobjint(name, "frametimer")
        local cx, cy, cz = getpos(name)

        if frametimer > 0 then
          frametimer = frametimer - 1
        end

        if frametimer == 0 then

          if frame > 15 then
            frame = 15
          end

          local m = ""

          if frame == 12 then
            m = "buffhorn_000120.obj"
          end
          if frame == 13 then
            m = "buffhorn_000130.obj"
          end
          if frame == 14 then
            m = "buffhorn_000140.obj"
          end
          if frame == 15 then
            m = "buffhorn_000150.obj"
            setobjint(name, "ignorecollisions", 1) 
            setobjstr(name, "state", "dead")

            local r = randint(1, 2)            
            if r == 1 then
              playsound("buffhornpain", true)
            else
              playsound("buffhorndeath", true)
            end
          end

          setmodel(name, m)

          frametimer = 2
         
          -- Change frame
          frame = frame + 1
          
        end

        cy = cy - 0.1    

        setobjint(name, "frame", frame)
        setobjint(name, "frametimer", frametimer)
        setpos(name, cx, cy, cz)

      end

    end
  end
end

-------------------------------
-- PROJECTILES
-------------------------------

function processprojectiles()

  -- Process projectiles

  for i = 1, numprojectiles, 1 do
  local name = "proj" .. i

    if getobjint(name, "exploding") ~= 1 then
      moveforward(name, 1)
    end

    local stuck = getobjint(name, "stuck")
    local stuckon = getobjstr(name, "stuckon")
    
    if stuck == 1 and getvisible(name) then
      setvisible(name, false)
      setlight(name, 0, 0, 0, 0)

      local hitpts = gethitpts(stuckon)
      if hitpts > 0 then      
        hitpts = hitpts - 30
        sethitpts(stuckon, hitpts)
        playsound("fireballexplosion", true)
        setobjint(name, "ignorecollisions", 1)

        if stuckon == "player" then
          if hitpts > 0 then
            setglobalcolor(3, 1, 1, 1)
            fadeglobalcolor(1, 1, 1, 1, 30)
          else
            setglobalcolor(3, 1, 1, 1)
            fadeglobalcolor(0, 0, 0, 1, areaswitchtimerdelay)
          end
        end

      end
    end

  end

end  

-------------------------------
-- OUTCOMES
-------------------------------

function processoutcomes()
 
--  log("areaswitchtimer " .. areaswitchtimer)

  if not switchingarea then

    -- Check for loss
 
    if gethitpts("player") <= 0 then
--      settext("message", "you lost")
--      playsound("explosion1", true)
      areaswitchtimer = areaswitchtimerdelay
      win = false
      switchingarea = true
    end

    -- Check for win

    if checkcoll("player", "exit", 5, 5, 5) then
--      settext("message", "you won")
      playsound("bell", true)
      areaswitchtimer = areaswitchtimerdelay
      win = true
      switchingarea = true
      setglobalcolor(1, 1, 1, 1)
      fadeglobalcolor(0, 0, 0, 1, areaswitchtimerdelay)
    end

  end
  
  -- Swtich area

  if areaswitchtimer > 0 then
    areaswitchtimer = areaswitchtimer - 1
  end

  if areaswitchtimer == 0 then
    switchingarea = false
    areaswitchtimer = -1
    settext("message", "")
    
    if win then
      curarea = curarea + 1
    end
    
    if curarea > numareas then
      curarea = 1
    end
    
    local fname = "area1.sc"
--    local fname = "area" .. math.floor(curarea) .. ".sc"
    
    start(fname)
  end

end

-------------------------------
-- MAIN TICK LOOP
-------------------------------

playerthink()
computerthink()
processprojectiles()
processoutcomes()
