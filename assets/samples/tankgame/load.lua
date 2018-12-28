curarea = 1
numareas = 3
win = false

--  Preload textures
for i = 1, 47, 1 do
  loadtex("expla" .. math.floor(i) .. ".png")
end

function start(scene)

clear()

sethealthbarsvisible(false)

-------------------------------
-- GLOBALS
-------------------------------

numprojectiles = 8
numcomputers = curarea

playerfiretimerdelay = 100
playerfiretimer = 0

armedproj = 1

switchingarea = false
areaswitchtimerdelay = 100
areaswitchtimer = -1

-------------------------------
-- COTNROLS
-------------------------------

setcontrolsvisible(true)

addwg("firebtn", "btn", "gamepadbutton.png", "", "", "", .7, .15, 0.2, 0.2)
setwgcolor("firebtn", 1, 1, 1, 0.5)
addtouchbtnbind(0, .7, .275, 0.3)

-------------------------------
-- TEXT
-------------------------------

addtext("message", "", 0.0, 0.3, 0.1, "")

-------------------------------
-- LOAD LEVEL
-------------------------------

loadscene(scene)
setmodelorient("sphere.obj", 0, 180, 0)
setmodelorient("sphere.obj", 0, 180, 0)

-------------------------------
-- COMPUTER
-------------------------------

for idx = 1, numcomputers, 1 do
    
  local name = "computer" .. idx

  -- The turret should rotate  
  setsecondaryyawmesh(name, 0)
  
  setobjint(name, "firetimer", 0)
  setvisible(name, true)

end

-------------------------------
-- PLAYER
-------------------------------

-- Set player params

setplayerobj("player")
setsecondaryyawmesh("player", 1)
setmovesmoothly("player", true)
setcamerascheme("thirdperson")
setcontrolscheme("thirdperson")
setplayerorient(0, 0, 0)

-------------------------------
-- CREATE PROJECTILES
-------------------------------

for i = 1, numprojectiles, 1 do
  local name = "proj" .. i

  addobj(name)
  settype(name, "sprite")
  settex(name, "ball.png")
  setpos(name, 0, -1000, 0)
  setsize(name, 1, 1, 1)
  setcolor(name, 0.2, 0.2, 0.2, 1.0)
--  setcolor(name, 3.0, 1.3, 0.5, 1.0)
  setvisible(name, false)
  setfade(name, 2000, 3000)
  setfaceplayer(name, 1)
  setobjint(name, "ignorecollisions", 1)
end

-------------------------------
-- GET LOADED OBJECTS READY
-------------------------------

getallobjs()

log(allobjs["player"])

for k, v in pairs(allobjs) do
  if v == "voxels" then
    sethitpts(k, 10)
  end
  if v == "model" then
    sethitpts(k, 10)
  end
  if v == "terrain" then
    local mult = 0.4
    local x, y, z = getpos(k)
    local sx, sy, sz = getsize(k)
    setplayarea(x - (sx * mult) / 2, x + (sx * mult) / 2, 0, 0, z - (sz * mult) / 2, z + (sz * mult) / 2)
  end
end

sethitpts("player", 10)
setyaw("player", 1)
setsecondaryyaw("player", 1)

setcamorient(0, 0, 0)

end

-------------------------------
-- RUN START FUNCTION
-------------------------------

start("area1.sc")