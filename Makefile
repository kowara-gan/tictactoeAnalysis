ALL : makeAllState makeWinLose checkState
CXX = g++
tictoctoe.o : tictoctoe.h
#
allStateTable.o : allStateTable.h tictoctoe.h
winLoseTable.o : winLoseTable.h tictoctoe.h
makeAllState.o : tictoctoe.h
makeAllState : makeAllState.o tictoctoe.o
	$(CXX) -o makeAllState makeAllState.o tictoctoe.o
makeWinLose.o : tictoctoe.h
allStateTable.o : allStateTable.h tictoctoe.h
makeWinLose : makeWinLose.o tictoctoe.o allStateTable.o
	$(CXX) -o makeWinLose makeWinLose.o tictoctoe.o allStateTable.o
checkState.o : tictoctoe.h
checkState : checkState.o tictoctoe.o allStateTable.o winLoseTable.o
	$(CXX) -o checkState checkState.o tictoctoe.o allStateTable.o winLoseTable.o
clean:
	-rm *.o