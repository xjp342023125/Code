a={}
b={__mode="k"}
setmetatable(a,b)

key={}
a[key]=1
print("------------")
for k,v in pairs(a) do print(v) end

key={}
collectgarbage()
print("------------")
for k,v in pairs(a) do print(v) end

a[key]=2
print("------------")
for k,v in pairs(a) do print(v) end