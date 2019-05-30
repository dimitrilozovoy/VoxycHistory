local px, py, pz = getpos("player")

if pz < -chunksize * (numchunks - 3) then
  start()
end