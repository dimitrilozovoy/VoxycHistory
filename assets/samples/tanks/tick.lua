
-------------------------------
-- PROJECTILES
-------------------------------

function processprojectiles()

  -- Process projectiles

  for i = 1, numprojectiles, 1 do
  local name = "proj" .. i

    if getobjint(name, "exploding") ~= 1 then
      moveforward(name, 7)
    end
  end
end  

-------------------------------
-- PLAYER
-------------------------------

function playerthink()

  if gethitpts("player") > 0 then

    if playerfiretimer > 0 then
      playerfiretimer = playerfiretimer - 1
    end

    if playerfiretimer == 0 and playerdead ~= 1 and playerdying ~= 1 and getbtn(0) == 1 then

      -- Fire

      local armedprojname = "proj" .. armedproj      
      local px, py, pz = getpos("player")
      setpos(armedprojname, px, py + 1, pz)      
    
      local apy = getsecondaryyaw("player")
      setyaw(armedprojname, apy) 
      setvisible(armedprojname, true)
      moveforward(armedprojname, 10)
    
      armedproj = armedproj + 1

      if armedproj >= numprojectiles then
        armedproj = 1
      end

      local r = randint(1, 8)
      playsound("cannon.ogg", true)
    
      playerfiretimer = 20
    end

  end

end

-------------------------------
-- COMPUTER
-------------------------------

function computerthink()

  for idx = 1, numcomputers, 1 do
    
    local name = "computer" .. idx

    if getvisible(name) then

      local cx, cy, cz = getpos(name)
      local destx = getobjint(name, "destx")
      local destz = getobjint(name, "destz")
    
      if getobjint(name, "stuck") == 1 or (destx == 0 or desty == 0) or (getobjint(name, "backingout") == 1 and getobjint(name, "backingouttimer") == 0) or (math.abs(destx - cx) < 10 and math.abs(destz - cz) < 10) then

       -- Find new destination

         setobjint(name, "backingout", 0)
         setobjint(name, "backingouttimer", 0)

         local px, py, pz = getplayerpos()
       
         destx = px + rand(-50, 50)
         destz = pz + rand(-50, 50)

         setobjint(name, "destx", destx)
         setobjint(name, "destz", destz)

      end

      if getobjint(name, "backingout") == 1 then
        setobjint(name, "backingouttimer", getobjint(name, "backingouttimer") - 1)
        moveforward(name, -0.2)
      else

        -- Turn towards destination and move

        local yawtodest = getyawtopoint(name, destx, destz)
        local yaw = getyaw(name)

        if not compareyaw(yaw, yawtodest, 10) then
          setyaw(name, rotateangletowards(getyaw(name), yawtodest, 5))
          moveforward(name, 0.1)
        else
          moveforward(name, 0.2)
        end
      end

      -- Attack

      local yawtotarget = getyawto(name, "player")   

      if not compareyaw(getsecondaryyaw(name), yawtotarget, 4) then    
          setsecondaryyaw(name, rotateangletowards(getsecondaryyaw(name), yawtotarget, 2))
      else
       
        if getobjint(name, "firetimer") > 0 then
          setobjint(name, "firetimer", getobjint(name, "firetimer") - 1)
        end

        if getobjint(name, "firetimer") == 0 then

          -- Fire

          local armedprojname = "proj" .. armedproj      
          local cx, cy, cz = getpos(name)
          setpos(armedprojname, cx, cy + 1, cz)      
    
          local acy = getsecondaryyaw(name)
          setyaw(armedprojname, acy) 
          setvisible(armedprojname, true)
          moveforward(armedprojname, 7)
    
          armedproj = armedproj + 1

          if armedproj >= numprojectiles then
            armedproj = 1
          end

          local r = randint(1, 8)
          playsound("cannonquiet.ogg", true)
    
          setobjint(name, "firetimer", 50)
        
        end
      end
    end
  end  
end

-------------------------------
-- COLLISIONS
-------------------------------

function processcollisions()

  --  
  -- Player projectiles hitting objects
  --

  for playerprojidx = 1, numprojectiles, 1 do
    
    local projname = "proj" .. playerprojidx
    
    if getvisible(projname) then
      for k, v in pairs(allobjs) do
        if getvisible(k) and checkcoll(projname, k) and getobjint(projname, "exploding") ~= 1 then
          if v == "voxels" or v == "model" then          

            playsound("explosion1.ogg", true)
            local newhitpts = gethitpts(k) - 1
	    sethitpts(k, newhitpts)

            -- Explode projectile
            setobjstr(projname, "oldtex", gettex(projname))
	    setobjint(projname, "exploding", 1)
	    setobjint(projname, "explframe", 1)
            setcolor(projname, 1.0, 1.0, 1.0, 1.0)
            setsize(projname, 4, 4, 4, 4)
            moveforward(projname, -10)

--            setvisible(projname, false)

            if newhitpts == 0 then
              -- Explode object
	      settype(k, "sprite")
              settex(k, "expla1.png")

              local sx, sy, sz = getsize(k)
              setsize(k, sx * 2, sy * 2, sz * 2)

              setfaceplayer(k, 1)
	      setobjint(k, "exploding", 1)
	      setobjint(k, "explframe", 1)
            end	    

          end
        end
      end
    end
  
  end

end
  

-------------------------------
-- EFFECTS
-------------------------------

function processeffects()

  --
  -- Exploding objects
  --

  for k, v in pairs(allobjs) do
    if v == "voxels" or v == "model" then        

        -- Explosion animation
	if getobjint(k, "exploding") == 1 then
	  local explframe = getobjint(k, "explframe")
          if explframe < 48 then
            settex(k, "expla" .. math.floor(explframe) .. ".png")
            setobjint(k, "explframe", explframe + 1)
          else
            setvisible(k, false)
          end
        end

    end
  end

  --
  -- Exploding projectiles
  --

  for playerprojidx = 1, numprojectiles, 1 do
    
    local projname = "proj" .. playerprojidx
  
    if getobjint(projname, "exploding") == 1 then
	  local explframe = getobjint(projname, "explframe")
          if explframe < 48 then
            settex(projname, "expla" .. math.floor(explframe) .. ".png")
            setobjint(projname, "explframe", explframe + 1)
          else
            -- Get projectile to be re-fired
            setvisible(projname, false)
            settex(projname, getobjstr(projname, "oldtex"))
            setobjint(projname, "exploding", 0)
            setsize(projname, 1, 1, 1)
            setcolor(projname, 0.2, 0.2, 0.2, 1.0)
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
  
    if gethitpts("player") == 0 then
      settext("message", "you lost")
      playsound("bell.ogg", true)
      areaswitchtimer = areaswitchtimerdelay
      win = false
      switchingarea = true
    end

    local numcompsalive = 0

    for idx = 1, numcomputers, 1 do

      local name = "computer" .. idx

      if gethitpts(name) > 0 then
        numcompsalive = numcompsalive + 1
      end

    end

    -- Check for win

    if numcompsalive == 0 then
      settext("message", "you won")
      playsound("bell.ogg", true)
      areaswitchtimer = areaswitchtimerdelay
      win = true
      switchingarea = true
    end

  end
  
  -- Swtich area

  if areaswitchtimer > 0 then
    areaswitchtimer = areaswitchtimer - 1
  end

  if areaswitchtimer == 0 then
    switchingarea = false
    switchareatimer = -1
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

processprojectiles()
playerthink()
computerthink()
processcollisions()
processeffects()
processoutcomes()
