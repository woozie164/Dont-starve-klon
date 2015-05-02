local gobj = GameObject_Create()

local foodObj = {}
foodObj["gobjptr"] = gobj 
foodObj["foodValue"] = 10

GameObject_SetPosition(gobj, 0.0, 3.0, 3.0)

if foodObj and foodObj.foodValue then
	print("The foodvalue is " .. foodObj["foodValue"])
end

function eat( eater, food ) 
	if food and food.foodValue then
		if eater and eater.hunger then
			eater.hunger = eater.hunger + food.foodValue
			print("nom nom")
			-- Remove food object
		else
			print("not an eater")
		end
	else 
		print("not food")
	end
end

function isfood( food ) 
	if food and food.foodValue then
		return true 
	else 
		return false 
	end 
end

function iseater( food )
	if eater and eater.hungerValue then
		return true
	else
		return false
	end
end


local testEater = {}
testEater["hunger"] = 100

eat(testEater, foodObj)
testEater["eat"] = eat
testEater:eat(foodObj)

local entities = {}
--entities[1] = testEater;
--entities[2] = foodObj;
entities[gobj] = foodObj;
local gobj2 = GameObject_Create()
entities[gobj2] = testEater;
print(gobj)
print(gobj2)

function oncollision( self, other )
	-- Search the table containing all game objects to find the
	-- game objects that match the values of the gobjptr sent as arguments
	--for key, value in ipairs(entities) do
	--	print(key .. " " .. value)
	--end
	eat(entities[self], entities[other])	
end

oncollision( gobj2, gobj )

Tile_Create(4.0, 4.0, 4.0, 4.0, 0.0)
Tile_Create(4.0, 4.0, 8.0, 4.0, 0.0)

