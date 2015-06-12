minGLUT.cpp
	Inneholder armadillo-solveren.
	Man kan endre grid size inne i myArmadilloClass.h og kompilere på nytt for hver gang.

	$ make arma	//kompilerer koden
	$ ./arma	//kjører koden


gauss_seidel.cpp
	Inneholder gauss-seidel solveren. Her må man bruke parameterliste når man kjører slik som dette:
	
	$ ./gs n k diff
	n = grid size, k = iterasjoner for gauss-seidel, diff = diffusjonskonstant
	
	Konstantene må stemme overens for å få gode animasjoner.
	Funker best for store grid siden museinput sprayer over flere grid-points.
	Kombinasjonene i oppskriften under funker bra.


	$ make gs
	$ ./gs 200 10 (dra musen litt og deretter PRESS d for å starte diffusion)
	$ ./gs 20 10 12 (liten grid)
	$ ./gs 64 10 5	
	$ ./gs 150 10 1


	Dere kan også prøve å kommentere ut linje 104 før kompilering for å se grid'en som ligger bak.
