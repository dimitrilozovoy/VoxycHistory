playerfiretimer = 0

-------------------------------
-- SPAWN
-------------------------------

function spawn_player()

  addobj("player")
  settype("player", "sprite")
  settex("player", "player.png")

  setpos("player", 0.0, 10.0, -50.0)
  setsize("player", 5.0, 5.0, 5.0)
  setcolor("player", 1.0, 1.0, 1.0, 1.0)
  sethitpts("player", 100)
  setmeshcolor("player", 1, 1.0, 1.0, 1.0, 0.5)
  setmovesmoothly("player", true)

  setplayerobj("player")
  setcontrolscheme("swipexy")
  setcamerascheme("thirdpersonfixed")

  setvelocity("player", 0, 0, -10)
  setplayerrangex(-60, 60)
  setplayerrangey(-60, 60)
  setplayermovespeed(0.05)
  
end

-------------------------------
-- TICK
-------------------------------

function tick_player()

  if playerfiretimer > 0 then
    playerfiretimer = playerfiretimer - 1
  end
  
  if playerfiretimer == 0 then
--    log("launch")
  
    local px, py, pz = getpos("player")
  
    launch_projectile("greenproj.png", px, py, pz, 0, 0.1, 0, 30)
    playerfiretimer = 10
  end
  
end