all:
	g++ -pthread main.cpp ADS_server.cpp client.cpp NR_server.cpp Root_server.cpp TLD_server.cpp -o run
test:
	g++ -pthread test.cpp -o run
clean:
	rm -rf run ./log_files/*