include Makefile.define

default:all

PATS1 = main.o ReadObject.o XmlOpeation.o

LIB_BASE_OBJS1 = main.o ReadObject.o XmlOpeation.o

LIB_BASE1 = ESCTools

all:$(LIB_BASE1) makefile

$(LIB_BASE1):$(PATS1)
	$(CC) -rdynamic -o $(LIB_BASE1) $(LIB_BASE_OBJS1) $(LIBS)

clean:
	rm -rf *.o  $(LIB_BASE1) $(LIB_BASE_OBJS1)
cl:
	rm -rf *.o 
