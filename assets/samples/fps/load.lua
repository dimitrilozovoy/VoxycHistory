setextraint("graphics", 1)

runscript("lights.lua")

showmouse(false)

-------------------------------
-- GLOBALS
-------------------------------

curarea = 1
numareas = 3
areatexttimer = 0
numcomputers = 0
numprojectiles = 5
armedproj = 1
playerattacktimer = 0

-------------------------------
-- PRELOAD ASSETS
-------------------------------

loadmodel("sphere.obj")

-------------------------------
-- COTNROLS
-------------------------------

setcontrolsvisible(true)

addwg("firebtn", "btn", "gamepadbutton.png", "", "", "", .7, .15, 0.2, 0.2)
setwgcolor("firebtn", 1, 1, 1, 0.5)
addtouchbtnbind(0, .7, .275, 0.3)

function start(scene)

  -- Preload models
  if scene == "area1.sc" then
    loadmodel("buffhorn_000010.obj")
    loadmodel("buffhorn_000020.obj")
    loadmodel("buffhorn_000030.obj")
    loadmodel("buffhorn_000040.obj")
    loadmodel("buffhorn_000050.obj")
    loadmodel("buffhorn_000060.obj")
    loadmodel("buffhorn_000070.obj")
    loadmodel("buffhorn_000080.obj")
    loadmodel("buffhorn_000120.obj")
    loadmodel("buffhorn_000130.obj")
    loadmodel("buffhorn_000140.obj")
    loadmodel("buffhorn_000150.obj")
  end

  if curarea == 1 then
    playtrack("rock1", true)
  end
  if curarea == 2 then
    playtrack("synth1", true)
  end
  if curarea == 3 then
    playtrack("galacticjourney", true)
  end

  -------------------------------
  -- RESET GLOBALS
  -------------------------------

  switchingarea = false
  areaswitchtimerdelay = 30
  areaswitchtimer = -1

  -------------------------------
  -- LOAD LEVEL
  -------------------------------

  setextraint("legacytexturespan", 1)

  clear()
  loadscene(scene)
  
  if curarea == 1 or curarea == 2 then
--    autobatch()
  end
  
  -------------------------------
  -- TEXT AND FADE
  -------------------------------

  addtext("health", "", -0.8, 0.8, 0.05, "")

  addtext("message", "", 0.0, 0.5, 0.1, "")

  addtext("area", "area " .. math.floor(curarea), 0.0, 0.3, 0.1, "")
  settext("area", "area " .. math.floor(curarea))
  areatexttimer = 60

  setglobalcolor(0, 0, 0, 1)
  fadeglobalcolor(1, 1, 1, 1, 30)

  -------------------------------
  -- PLAYER
  -------------------------------

  setcamerascheme("fps")
  setcontrolscheme("fps")
  sethealthbarsvisible(false)

  -- Spawn player
  local psx, psy, psz = getpos("playerspawn")
  setplayerpos(psx, psy, psz)
  setplayersize(1.5, 3, 1.5)
  setvisible("playerspawn", false)
  sethitpts("player", 50)
--  setmovesmoothly("player", true)

  setvisible("exit", false)

  -- Weapon
  addobj("weapon")
  settype("weapon", "model")
  setmodel("weapon", "sphere.obj")
  settex("weapon", "bluecube.png")
  setcolor("weapon", 1, 1, 1, 1)
  setsize("weapon", 0.2, 0.2, 0.2)
  setmodelorient("sphere.obj", 0, 90, 0)
--  setmovesmoothly("weapon", true)
  setobjint("weapon", "ignorecollisions", 1)

  -- Weapon fire
  addobj("weaponfire")
  settype("weaponfire", "sprite")
  settex("weaponfire", "shotgunfire.png")
  setcolor("weaponfire", 1, 1, 1, 1)
  setsize("weaponfire", 0.25, 0.25, 0.25)
  setfaceplayer("weaponfire", 1)
--  setmovesmoothly("weaponfire", true)
  setobjint("weaponfire", "ignorecollisions", 1)
  setvisible("weaponfire", false)

  -------------------------------
  -- COMPUTERS
  -------------------------------

  -- Turn all objects with a specific model name into computers

  getallobjs()

  local idx = 1

  for k, v in pairs(allobjs) do
    if v == "model" then
      local modelname = getmodel(k)
      local newname = "computer" .. idx

      if modelname == "buffhorn_000000.obj" then
        rename(k, newname)
        setphyssize(newname, 1.5, 5, 1.5)
        setobjstr(newname, "state", "move")
        setobjint(newname, "attacktimer", 0)
        setobjint(newname, "destx", 0)
        setobjint(newname, "destz", 0)
        setobjint(newname, "frame", 1)
        setobjint(newname, "frametimer", 0)
	sethitpts(newname, 20)
        idx = idx + 1 
      end
      
    end

  end

  numcomputers = idx

  spawn_lights()

  -------------------------------
  -- CREATE PROJECTILES
  -------------------------------

  for i = 1, numprojectiles, 1 do
    local name = "proj" .. i

    addobj(name)
    settype(name, "sprite")
    settex(name, "fireball.png")
    setpos(name, 0, -1000, 0)
    setsize(name, 1, 1, 1)
    setcolor(name, 1.0, 1.0, 1.0, 1.0)
    setvisible(name, false)
    setfade(name, 2000, 3000)
    setfaceplayer(name, 1)
--    setobjint(name, "ignorecollisions", 1)
  end

end

-------------------------------
-- RUN START FUNCTION
-------------------------------

start("area" .. math.floor(curarea) .. ".sc")
