A = {v=1}

function A:f(v_)
	self.v = self.v+v_
end

a1=A
a2=A
print(a1.v,a2.v)
a1.f(a1,10)
print(a1.v,a2.v)
a1:f(10)
print(a1.v,a2.v)
-- a1:f(10)  == a1.f(a1,10)
-----------------------------------
function A:new(o)
	print(A,self)
	o = o or {}
	
	--[[
	local temp={}
	setmetatable(o,temp)
	temp.__index=A
	]]--
	
	---[[
	setmetatable(o,self)
	self.__index=self
	
	--]]--
	
	return o
end

a3=A:new({t=2})
a3:f(5)
a3:f(5)
print(a3.v,a3.t,a3.t2)


a4=A:new{t=2}
a4:f(5)
print(a4.v,a4.t,a4.t2)
print("-==================================")


--[[
function f2(self,v_)
	self.v = self.v+v_
end
--b={}    lua: class.lua:27: attempt to perform arithmetic on a nil value (field 'v')
b={v=20}
f2(b,10)
print(b.v)
]]--