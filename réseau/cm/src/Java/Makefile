JAVAC = javac
PRG = ClientSocketTCP.class    \
      ProcessConnection.class  \
      ServerSocketUDP.class    \
      ClientSocketUDP.class    \
      ServerSocketTCP.class

all: $(PRG)

%.class: %.java
	$(JAVAC) $<

clean:
	$(RM) $(PRG) *~
