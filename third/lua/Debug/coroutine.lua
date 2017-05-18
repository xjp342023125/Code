co1 = coroutine.create(function() print("hi") end)
print(co1)
print(coroutine.status(co1))

coroutine.resume(co1)

print(coroutine.status(co1))

co2 = coroutine.create(function()
	for i=1,10 do 
		print("co",i)
		coroutine.yield()
	end
end
)

coroutine.resume(co2)
coroutine.resume(co2)
coroutine.resume(co2)

print("=============================")
threads = {}
function test_thread(key)
	local co = coroutine.create(
	function()
		local idx = 0
		
		print(key,"run",idx)
		idx = idx+1
		coroutine.yield()
		
		print(key,"run",idx)
		idx = idx+1
		coroutine.yield()
		
		print(key,"run",idx)
		idx = idx+1
		coroutine.yield()
	end
	)
	table.insert(threads,co)
	print("new thread")
end
		
		
test_thread("111")
test_thread("222")
test_thread("333")

for i=1,3 do
	for k,v in pairs(threads) do
		coroutine.resume(v)
	end
	
end