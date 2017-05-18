function f0() end
function f1() return "a" end
function f2() return "a","b" end

x,y = f2()
print(x,y)
x=f2()
print(x)
x,y = (f2())
print(x,y)