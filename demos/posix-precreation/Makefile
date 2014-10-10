CC=gcc

SOURCE=program.c
TARGET=program

SOURCE2=prepare.c
TARGET2=prepare

all:
	${CC} -o ${TARGET} ${SOURCE}
	${CC} -o ${TARGET2} ${SOURCE2}

clean:
	rm -f test.tmp
	rm -f ${TARGET}
	rm -f ${TARGET2}
	rm -rf mytrace
