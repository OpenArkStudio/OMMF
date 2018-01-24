include Makefile.define

default:all

PATS1 = ./TinyXML/tinyxml.o ./TinyXML/tinyxmlerror.o ./TinyXML/tinyxmlparser.o ./TinyXML/tinystr.o main.o ReadObject.o XmlOpeation.o 

LIB_BASE_OBJS1 = tinyxml.o tinyxmlerror.o tinyxmlparser.o tinystr.o main.o ReadObject.o XmlOpeation.o 

LIB_BASE1 = ESCTools

all:$(LIB_BASE1) Makefile

$(LIB_BASE1):$(PATS1)
	$(CC) -rdynamic -o $(LIB_BASE1) $(LIB_BASE_OBJS1) $(LIBS)

clean:
	rm -rf *.o  $(LIB_BASE1) $(LIB_BASE_OBJS1)
cl:
	rm -rf *.o 
