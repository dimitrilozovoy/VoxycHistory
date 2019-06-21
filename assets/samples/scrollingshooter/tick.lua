--fixedtimestep(true)

tick_explosions()
tick_projectiles()
tick_computers()
tick_player()

local px, py, pz = getpos("player")

if pz < -chunksize * (numchunks - 3) then
  start()
end