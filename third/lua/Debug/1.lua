
b={x=10,y=45;"1","2","3"}
for i=1,#b do print(b[i]) end
print("=====================")
poly={x1=100,x2=200,x3=300,
{x=1,y=0},
{x=2,y=1},
{x=3,y=2}
}

print(poly[1].x)
print(poly[2].x)
print(poly[3].x)
print(poly[0])
print("=====================")






a={x=10,y=20}

print(a.x)
print(a.y)
a["x"]=200
print(a.x)


print(0 and 1 and nil)


function foo(a, b, c)
 local sum = a + b
 return sum, c --函数可以返回多个值
end
r1, r2 = foo(1, '123', 'hello') --平行赋值
print(r1, r2)
