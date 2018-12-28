for i = 0, 100, 1 do

  local x = rand(-400, 400)
  local y = 4
  local z = rand(-400, 400)
  
  if (x > 100 or x < -100) and (z > 100 or z < -100) then
    local name = "tree" .. math.floor(i)
    addobj(name)
    setpos(name, x, y, z)
    settype(name, "sprite")
    settex(name, "tree1.png")
    setsize(name, 4, 4, 4)
    setfaceplayer(name)
  end
  
end