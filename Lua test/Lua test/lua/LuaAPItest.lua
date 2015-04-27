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
		end
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

function oncollision( self, other )
	if iseater(self) and isfood(other) then
		print("Ate someting")
	end	
end

