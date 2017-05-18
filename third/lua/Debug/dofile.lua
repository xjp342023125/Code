print('dofile')
dofile('assignment.lua')

local i = 0;
print(i)
loadstring("i = i+1")()
print(i)