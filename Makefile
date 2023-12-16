CXX = g++
CXXFLAGS = -std=c++20 -Wall -MMD -g -O3
EXEC = age
OBJECTS = main.o movement.o entity.o view.o age.o controller.o collision.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lncurses

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
