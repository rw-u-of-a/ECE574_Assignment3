input Int32 a, b, c
output Int32 z, x

variable Int32 d, f, g, zrin

d = a + b			//1
g = a < b			//2
zrin = a + c		//3	
if ( g ) {
	zrin = a + b	//4	
}
f = a * c			//5
x = f - d  			//6
z = zrin + f		//7