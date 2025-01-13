compile clientRun serverRun: forking_server.o persistant_client.o pipe_networking.o
	gcc -o clientRun persistant_client.o pipe_networking.o
	gcc -o serverRun forking_server.o pipe_networking.o
client: clientRun
	./clientRun
server: serverRun
	./serverRun
persistant_client.o: persistant_client.c pipe_networking.h
	gcc -c persistant_client.c
forking_server.o: forking_server.c pipe_networking.h
	gcc -c forking_server.c
pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c
clean:
	rm -f mario
	rm -f *.o
	rm -f *~
	rm -rf clientRun
	rm -rf serverRun
