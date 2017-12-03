input Int32 a, b, c, one
output Int32 z, x

variable Int32 d, e, f, g, h 
variable Int1 dLTe, dEQe, dLTEe

d = a + b				//1
e = a + c				//2
f = a - b  				//3
dEQe = d == e			//4
dLTe = d > e			//5
dLTEe = dEQe + dLTe		//6

if ( dLTEe ) {

	if ( dLTe ) {
		g = e + one		//7
		h = f + one		//8
	}
	g = d + e			//9
	h = f + e			//10
}
x = h << one			//11
z = h >> one			//12
