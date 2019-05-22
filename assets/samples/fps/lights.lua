function spawn_lights()
  local idx = 1

  for k, v in pairs(allobjs) do
    if v == "sprite" then
      local tex = gettex(k)
      local newname = "light" .. idx

      if tex == "orangelight.png" then
        rename(k, newname)
        setlight(newname, 10, 3, 2, 1)
        setobjint(newname, "ignorecollisions", 1)
        idx = idx + 1 
      end
      
      if tex == "bluelight.png" then
        rename(k, newname)
        setlight(newname, 7.5, 1, 2, 3)
        setobjint(newname, "ignorecollisions", 1)
        idx = idx + 1 
      end

    end
  end
end