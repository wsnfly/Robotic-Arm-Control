import math
a = 1000
u = 500
s = 0
mc = []
for s in range(2001):
    
    v = math.sqrt(u*u + 2 * a * s)
    mc.append(int( 32000000 / v))
print((mc))
    