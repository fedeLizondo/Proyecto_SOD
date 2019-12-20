#Makefile
OBJS = serverSwitch serverFireBird serverMysql serverPosgres
CFLAGS = -g -Wall -Wextra
LDFLAGS = -lpthread#-pthread -lpthread
CPPFLAGS = Servidor/servidor.c Servidor/client.c

all:install
#install: $(OBJS)
#	@echo "Done"
#	gcc $(CPPFLAGS) $(LDFLAGS) $< -o $@ 
install: $(OBJS)
	@echo "Done"
	
serverSwitch: serverSwitch.o
	gcc $(CPPFLAGS) $(LDFLAGS) $< -o $@ 
serverFireBird: serverFireBird.o
	gcc $(CPPFLAGS) $(LDFLAGS) $< -o $@ -I . -L . -lfb
serverMysql: serverMysql.o
	gcc $(CPPFLAGS) $(LDFLAGS) $< -o $@ `mysql_config --cflags --libs` 
serverPosgres: serverPosgres.o
	gcc $(CPPFLAGS) $(LDFLAGS) $< -o $@ -I /usr/include/postgresql -lpq

%.o: %.c 
	gcc $(CFLAGS) -c $< -o $@ 

delete:
	rm -f *.o $(OBJS)

clean:
	rm -f *.o
	