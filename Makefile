CXX = g++
CXXFLAGS = -Wall -Wextra -Werror

CLIENT_SRC = src/client/main.cpp
SERVER_SRC = src/server/main.cpp

CLIENT_BIN = client.exe
SERVER_BIN = server.exe

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN):
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN)

$(SERVER_BIN):
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN)

clean:
	rm -f *.o

fclean: clean
	rm -f $(CLIENT_BIN) $(SERVER_BIN)

re: fclean all