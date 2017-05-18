--__index:access not exit
--__newindex:update not exit


t={x=100,y=200}
local _t = t
t={}
local mt={
	__index=function(t,k)
		print("*access "..tostring(k))
		return _t[k]
	end,
	
	__newindex=function(t,k,v)
		print("*uodate "..tostring(k).." TO "..tostring(v).." old="..tostring(_t[k]))
		_t[k]=v
	end
}


setmetatable(t,mt)




t['x']=1
t['x']=2
--print(t[2])
--t['111']='123'
--print(t['111'])

----------------------------------------------------------------------------------
function readOnly(t)
	local proxy={}
	local mt={
		__index = t,
		__newindex=function(t,k,v)
			error("read only",2)
		end
	}
	setmetatable(proxy,mt)
	return proxy
end

days=readOnly{"1","2"}

print(days[1])
days[1]="111"
	
	


