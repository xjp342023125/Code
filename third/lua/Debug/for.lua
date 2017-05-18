a={x=1,y=2,z=3}
for k,v in pairs(a) do print(k,v) end

a={"a","b","c"}
for k,v in ipairs(a) do print(k,v) end
print("==========")
a={x=1,y=2,z=3,"a","b","c"}
for k,v in pairs(a) do print(k,v) end