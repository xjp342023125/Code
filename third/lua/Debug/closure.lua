local function newCnt()
local i = 0
	return function ()
		i = i+1
		return i
		end
end

c1 = newCnt()
c2 = newCnt()


print(c1())
print(c1())
print(c1())

print(c2())
print(c2())