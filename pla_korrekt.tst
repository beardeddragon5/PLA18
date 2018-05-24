	const c = 6, d = 7, H = 72, E = 69, L = 76, O = 79, NL = 10;
	var n: int , f: real, e:real;
	procedure test;
		var i : int, k:real;
		procedure one;
			n := 0;
		begin
			f := k+f;
			i := 2;
			while i <=n do
			begin
				f := f +  k * 1.0;
				f := 1.0;
				if i = 0
					then i := 1 fi;
				i := i+1
			end
		end;
	begin
		if n = 0 then n := n + 1 else n := 0 fi;
		n := c;
		call test;

		cint := n;
		ascii := NL;

		ascii := H;
		ascii := E;
		ascii := L;
		ascii := L;
		ascii := O;
		ascii := NL;

		argIdx := 0;
		while argIdx < argCount do
		begin
			call arg;
			ascii := argOut;
			argIdx := argIdx + 1
		end
	end$
