math.randomseed(os.time())

local gobjptr = GameObject_Create()
local x = math.random(0, 24)
local z = math.random(0, 24)
GameObject_SetPosition(gobjptr, x, 3.0, z)
local food = {}
food["foodValue"] = 30
food["gobjptr"] = gobjptr
entities[gobjptr] = food
return gobjptr