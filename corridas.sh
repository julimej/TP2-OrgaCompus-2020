for VARIABLE in 1 2 3 4 5 6
do
    echo
    echo ejercicio $VARIABLE
    FILENAME=prueba$VARIABLE.mem
	./tp2 -v $FILENAME
done