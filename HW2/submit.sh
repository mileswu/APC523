STUFF="Makefile matrix.cc matrix.h minimization.cc problem1.cc problem2.cc vector.cc vector.h writeup/problem1.pdf"

rm -r submission
for i in $STUFF
do
	cp "$i" submission/
done

mv submission "MilesWu-HW2"
tar cjvf MilesWu-HW2.tar.bz2 MilesWu-HW2
 

