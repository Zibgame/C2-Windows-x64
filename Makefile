CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Isrc/server/class
LDFLAGS = -lws2_32 -lkernel32

CLIENT_SRC = src/client/main.cpp
SERVER_SRC = src/server/main.cpp \
             src/server/class/client.cpp \
             src/server/class/server.cpp

CLIENT_BIN = client.exe
SERVER_BIN = server.exe

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN):
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN) $(LDFLAGS)

$(SERVER_BIN):
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) $(LDFLAGS)

clean:
	del /Q *.o 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: fclean all