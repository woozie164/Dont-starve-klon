print('Hello script')
print( min_variabel )
min_variabel = "LUA"

sqr = function(x)
	return x * x
end

function errhandler( errmsg )
	return errmsg .. "\n" .. debug.traceback()
end

return "RETURN VALUE"



