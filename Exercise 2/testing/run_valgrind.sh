#Recompiles the program from scratch, just in case.
make clean
make

#Compiles the sensor
make sensor -s

#Creates a FIFO in the temp folder.
FIFO="/tmp/feedback_fifo"
mkfifo $FIFO

#Runs the programs in parallel using FIFO and Valgrind.
(valgrind --leak-check=full --track-origins=yes --verbose ./out < $FIFO | ./sensor $1 > $FIFO) &

#Waits until both have finished.
wait

#Removes the file.
rm $FIFO