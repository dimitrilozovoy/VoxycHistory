
-------------------------------
-- AREA / TERRAIN
-------------------------------

function switcharea(to)
  cleararea(area)
  
  area = to
  
  if area > areas then
    area = 1
  end
  
  buildarea(area)
  
  setmovesmoothly("player", 0)
  setpos("player", 0.0, 10.0, 0.0)
  setmovesmoothly("player", 1)
  
  settext("area", "area " .. area)
  settextvisible("area", true)
  areatexttimer = areatexttimerdelay
  
  if reviveplayeronareaswitch == 1 then
    log("reviveplayeronareaswitch")
  
    settype("player", "model")
    setmodel("player", "sphere.obj")
    settex("player", "bluecube.png")
    setvisible("player", true)

    setvisible("player", true)
    sethitpts("player", playerhitpts)
    setcolor("player", 1.0, 1.0, 1.0, 1.0)

    playerdead = 0
	playerrevivetimer = 0
    playerscore = 0
		  
	settext("score", "score " .. playerscore)

	reviveplayeronareaswitch = 0
  end
  
  fadeglobalcolor(1, 1, 1, 1, areaswitchtimerdelay)
end

function moveterrain()

		-- Move objects for endless terrain on Z axis
	
		local px, py, pz = getpos("player")

		lastblockz = curblockz

		if pz >= startz then
			curblockz = ((pz - startz) / blocksize)
		else
			curblockz = -(math.abs(pz - startz) / blocksize) - 1
		end

		if lastblockz ~= curblockz then
		    local fromblockz
			local toblockz

			if lastblockz > curblockz then
				fromblockz = lastblockz + numblocks / 2
				toblockz = curblockz - numblocks / 2
			end

			if lastblockz < curblockz then
				fromblockz = lastblockz - numblocks / 2
				toblockz = curblockz + numblocks / 2
			end

			moveobjsz(startz + fromblockz * blocksize + blocksize / 2.0, blocksize / 2.0, startz + toblockz * blocksize + blocksize / 2.0)
		end

		-- Move objects from side to side for endless terrain on X axis

		lastblockx = curblockx

		if px >= startx then
			curblockx = ((px - startx) / blocksize)
		else
			curblockx = -(math.abs(px - startx) / blocksize) - 1
		end

		if lastblockx ~= curblockx then
			local fromblockx
			local toblockx

			if lastblockx > curblockx then
				fromblockx = lastblockx + numblocks / 2
				toblockx = curblockx - numblocks / 2
			end

			if lastblockx < curblockx then
				fromblockx = lastblockx - numblocks / 2
				toblockx = curblockx + numblocks / 2
			end

			moveobjsx(startx + fromblockx * blocksize + blocksize / 2.0, blocksize / 2.0, startx + toblockx * blocksize + blocksize / 2.0)
		end
end

-------------------------------
-- COMPUTER
-------------------------------

function compchaseplayer(which)

  local name = "computer" .. which

  local x, y, z = getpos("computer" .. which)
--  z = z + 10
  
  local type = getobjint(name, "type")
  log("comp type " .. type)
  
  local px, py, pz = getpos("player")
  
  if type == 1 then
    if x > px then
        setextraint("compdelta" .. which, -3)
    end
  
    if x < px then
        setextraint("compdelta" .. which, 3)
    end
  end
end

function repositioncomps()
  local px, py, pz = getpos("player")

  for i = 1, numcomps, 1 do
    local name = "computer" .. i
    local x, y, z = getpos(name)
    
    if z > pz then
--      local nx = 0
      local nix = randint(-5, 5)
      local nx = px + nix * 20
      local ny = rand(0, 50)
      local nz = pz - 1000 + rand(0, 100)
--      log("setposx " .. nx)
      setpos(name, nx, ny, nz)

      settype(name, "model")

      local type = 1

      if area >= 4 then
        type = 2
      elseif area >= 2 then
        local r = randint(1, 5)
        if r > 1 then
          type = 1
        else
          type = 2
        end
      elseif area == 1 then
        type = 1
      end
      
      setobjint(name, "type", type)
        
      if type == 1 then
        setmodel(name, "sphere.obj")
        settex(name, "bluecube.png")
        setsize(name, 10, 10, 10)
      end
        
      if type == 2 then
        setmodel(name, "sphere.obj")
        settex(name, "")
        setsize(name, 20, 20, 20)
      end

--      compchaseplayer(i)
      
      -- Bring computer back to life
      if compdead[i] == 1 then
        compdead[i] = 0
        compdying[i] = 0
        setvisible(name, true)
        setcolor(name, 1, 1, 1, 1)
--        log("revived comp " .. i)
      end
    end
  end
end

function compthink(which)

--  log("compthink")

  if getextraint("compdelta" .. which) == 0 then
--    compchaseplayer(which)
  end
  
  local type = getobjint("computer" .. which, "type")

  local x, y, z = getpos("computer" .. which)
  
  if type == 1 then
    z = z + 30
  end
  if type == 2 then
    z = z + 10
  end
  
  x = x + getextraint("compdelta" .. which)
  
  setpos("computer" .. which, x, y, z)
  
  -- Process computer projectiles
  for i = 1, numprojectilespercomp, 1 do
    local name = "computer" .. which .. "proj" .. i

--    local pjx, pjy, pjz = getpos(name)
--    pjz = pjz + 30
--    setpos(name, pjx, pjy, pjz)

    moveforward(name, compprojspeed);

--    local pjyaw = getyaw(name)
--    setdeltaxz(name, pjyaw, compprojspeed)
--    move(name)
    
--    log("pjyaw " .. getyaw(name))

    if distance("player", "name") < 10 then
      setvisible("name", false)
    else
      setvisible("name", true)
    end
  end

  if compdead[which] == 1 or compdying[which] == 1 then
    return
  end
  
  -- Fire?
  
--  if compfiretimer[which] == nil then
--    log("nil at " .. which)
--  end

  local cx, cy, cz = getpos("computer" .. which)
  local px, py, pz = getpos("player")

  local firechance = 20
  local compfiretimerdelay = 100
  
  if type == 1 then
    firechance = 50
    compfiretimerdelay = compfuretimerdelay1
  elseif type == 2 then
    firechance = 10
    compfiretimerdelay = compfuretimerdelay2
  end

  if compfiretimer[which] ~= nil and compfiretimer[which] > 0 then
    compfiretimer[which] = compfiretimer[which] - 1
  elseif randint(1, firechance) == 1 and playerdead ~= 1 and playerdying ~= 1 and cz < pz - 200 then
    -- Fire
    local armedprojname = "computer" .. which .. "proj" .. comparmedproj[which]
          
    setpos(armedprojname, cx, cy, cz)
      
    local apy = getyawto(armedprojname, "player")
    local app = getpitchto(armedprojname, "player")

--    log("apy " .. apy)

    setyaw(armedprojname, apy)
    setpitch(armedprojname, app)
 
    local cp = getpitch("computer" .. which)
    local cr = getroll("computer" .. which)

--    setpitch(armedprojname, cp)
    setroll(armedprojname, cr)
      
    setvisible(armedprojname, true)

    comparmedproj[which] = comparmedproj[which] + 1

    if comparmedproj[which] >= numprojectilespercomp then
      comparmedproj[which] = 1
      compfiretimer[which] = compfiretimerdelay
    end
  end
 
end

-------------------------------
-- PLAYER
-------------------------------

function playerthink()
  -- Process player projectiles
  for i = 1, numplayerprojectiles, 1 do
  local name = "playerproj" .. i

    local pjx, pjy, pjz = getpos(name)
    pjz = pjz - 60
    setpos(name, pjx, pjy, pjz)

--  local cyaw = getyaw(name)
--  setdeltaxz(name, cyaw, compprojspeed)
--  move(name)

    if distance("player", "name") < 50 then
      setvisible("name", false)
    else
      setvisible("name", true)
    end
  end
  
  if playerfiretimer ~= nil and playerfiretimer > 0 then
    playerfiretimer = playerfiretimer - 1
  elseif randint(1, 1) == 1 and playerdead ~= 1 and playerdying ~= 1 then
    -- Fire
    local armedprojname = "playerproj" .. playerarmedproj
      
    local px, py, pz = getpos("player")
      
    setpos(armedprojname, px, py, pz - 30)
      
    apy = 0

    setyaw(armedprojname, apy)
 
--    local pp = getpitch("player")
--    local pr = getroll("player")

--    setpitch(armedprojname, pp)
--    setroll(armedprojname, pr)
      
    setvisible(armedprojname, true)
    
    playerfiretimer = 5
    playerarmedproj = playerarmedproj + 1

    local r = randint(1, 8)
    playsound("attack" .. math.floor(r))
    
--    log("playerarmedproj " .. playerarmedproj)

    if playerarmedproj >= numplayerprojectiles then
      playerarmedproj = 1
    end
  end
end

-------------------------------
-- COLLISIONS
-------------------------------

function processcollisions()
  for compidx = 1, numcomps, 1 do
    -- Player and computer collision
    if checkcoll("player", "computer" .. compidx, 0.5, 0.5, 1.0) then
      playsound("bump1.ogg", false)
      sethitpts("player", gethitpts("player") - 1)
      playerhurttimer = 100
    end
    
    -- Player hitting voxel
    if checkvoxcoll("player", 0.3, 0.5, 1) then
      sethitpts("player", 0)
    end

    -- Computer projectiles hitting player
    for compprojidx = 1, numprojectilespercomp, 1 do
      if getvisible("computer" .. compidx .. "proj" .. compprojidx) and checkcoll("player", "computer" .. compidx .. "proj" .. compprojidx, 0.5, 0.5, 2.5) then
        playsound("bump1.ogg", false)
        sethitpts("player", gethitpts("player") - 1)
        playerhurttimer = 100
        setvisible("computer" .. compidx .. "proj" .. compprojidx, false)
--        log("playerhurttimerdelay " .. playerhurttimerdelay)
      end
    end

    -- Player projectiles hitting computer
    for playerprojidx = 1, numplayerprojectiles, 1 do
      if checkcoll("computer" .. compidx, "player" .. "proj" .. playerprojidx, 1.0, 1.0, 2.5) then
        playsound("bump1.ogg", false)
        sethitpts("computer" .. compidx, 0)
        setvisible("player" .. "proj" .. playerprojidx, false)
--        log("hit!")

        if compdying[compidx] == 0 and compdead[compidx] == 0 then
          
          playerscore = playerscore + 1
          
          compdying[compidx] = 1
          compdyingtimer[compidx] = compdyingtimerdelay
          
          settext("score", "score " .. playerscore)
--          log("dying " .. compidx .. " timer " .. compdyingtimerdelay)
        end
      end
    end
    
    if compdying[compidx] == 1 then
      if compdyingtimer[compidx] > 0 then
--        log("compdyingtimer-- " .. compidx .. " " .. compdyingtimer[compidx])
        compdyingtimer[compidx] = compdyingtimer[compidx] - 1
      elseif compdyingtimer[compidx] <= 0 then
        compdying[compidx] = 0
        compdead[compidx] = 1
        setvisible("computer" .. compidx, false)    
--        log("compdead" .. compidx)    
      end
    end
  end

  if playerhurttimer > 0 then
    playerhurttimer = playerhurttimer - 1
--    log("playerhurttimer " .. playerhurttimer)
  end
  
  -- Player dying
  if playerdying == 1 then
	   if playerdyingtimer > 0 then
	     playerdyingtimer = playerdyingtimer - 1
	   elseif playerdyingtimer <= 0 then
	     playerdying = 0
	     playerdead = 1
	     setvisible("player", false)
	     playerrevivetimer = playerevivetimerdelay
	     settext("score", "score " .. playerscore)
    end
  end
  
  if gethitpts("player") <= 0 and playerdying == 0 and playerdead == 0 then
    playsound("explosion1.ogg", false)
	   playerdying = 1
    playerdyingtimer = playerdyingtimerdelay
  end

  -- Reviving player
  if playerdead == 1 and reviveplayeronareaswitch == 0 then
--    log("numcomps " .. numcomps)
--	    if playerrevivetimer > 0 then 
--	    playerrevivetimer = playerrevivetimer - 1
--	    if playerrevivetimer <= 0 then
    reviveplayeronareaswitch = 1
    log("reviveplayeronareaswitch = 1")
		  
		  areaswitchtimer = areaswitchtimerdelay
    fadeglobalcolor(0, 0, 0, 1, areaswitchtimerdelay)
    nextarea = 1
--    end
--	end
  end
end

-------------------------------
-- EFFECTS
-------------------------------

function processeffects()
--  setcolor("player", 1.0, 1.0, 1.0, 1.0)

  -- Player hurt
  if playerhurttimer > 0 then
    
    local redfactor = playerhurttimer / 100
--    log("playerhurttimer" .. playerhurttimer)
--    log("playerhurttimerdelay" .. playerhurttimerdelay)
    
    setcolor("player", 1.0 + redfactor, 1.0 + redfactor / 4, 1.0, 1.0)
  end

  -- Player dying
  if playerdying == 1 then
      settype("player", "sprite")
--      setshape("player", "sprite")
      local animlength = 47
      local animtimer = playerdyingtimer - (playerdyingtimerdelay - 47)
      if animtimer >= 0 then
--        log("expla" .. math.floor(animlength - animtimer) .. ".png")
        settex("player", "expla" .. math.floor(animlength - animtimer) .. ".png")
      else
        setvisible("player", false)
      end
--    setcolor("player", 1.0, 1.0, 1.0, playerdyingtimer / playerdyingtimerdelay)
--    setcolor("player", (playerdyingtimer / playerdyingtimerdelay), 1.0 - (playerdyingtimer / playerdyingtimerdelay), 1.0 - (playerdyingtimer / playerdyingtimerdelay), playerdyingtimer / playerdyingtimerdelay)
--  elseif playerdead == 1 and playerrevivetimer > 0 and playerrevivetimer < 50 then
--      settype("player", "model")
--      setmodel("player", "ufo.obj")
--      settex("player", "ufo.png")
--      setvisible("player", true)
--      setcolor("player", 1.0, 1.0, 1.0, 1.0 - (playerrevivetimer / 50.0))
  end

  -- Comptuer dying
  for compidx = 1, numcomps, 1 do
    if compdying[compidx] == 1 then
      settype("computer" .. compidx, "sprite")
--      setshape("computer" .. compidx, "sprite")
      settex("computer" .. compidx, "expla" .. math.floor(compdyingtimerdelay - compdyingtimer[compidx]) .. ".png")
    end
  end
end

-------------------------------
-- TICK MAIN
-------------------------------

if getextraint("menuclicked") == 1 then
  -- Switch to different module
  setextraint("switchmodule", 1)
  setextraint("nextmodule", 10)
  setextrastr("loadscript", "load.lua")
  setextrastr("tickscript", "tick.lua")
  setextraint("menuclicked", 0)
end

repositioncomps()
moveterrain()

for i = 1, numcomps, 1 do
--  log("compthink " .. i)
  compthink(i)
end

playerthink()
processcollisions()
processeffects()

-- Area switch

local px, py, pz = getpos("player")

if areaswitchtimer == -1 and pz < -areasize and playerdead ~= 1 and playerdying ~= 1 then
  areaswitchtimer = areaswitchtimerdelay
  fadeglobalcolor(0.0, 0.0, 0.0, 1.0, areaswitchtimerdelay)
--  fadeglobalcolor(0, 0, 0, 1, 10)
  nextarea = area + 1
end

if areaswitchtimer > 0 then
  areaswitchtimer = areaswitchtimer - 1
end

if areaswitchtimer == 0 then
  switcharea(nextarea)
  areaswitchtimer = -1
end

areatexttimer = areatexttimer - 1

if areatexttimer < 0 then
  settextvisible("area", false)
end
