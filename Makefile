CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -static -static-libgcc -static-libstdc++ -Isrc/server/class -Isrc/server/cli
LDFLAGS = -lws2_32 -lkernel32

CONFIG = config.txt

CLIENT_SRC = src/client/main.cpp
CLIENT_TMP = client_tmp.cpp
CLIENT_BIN = client.exe

SERVER_SRC = src/server/main.cpp \
             src/server/class/client.cpp \
             src/server/class/server.cpp \
             src/server/cli/cli.cpp

SERVER_BIN = server.exe

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN):
	powershell -Command " \
	if (Test-Path $(CONFIG)) { \
		$$c = Get-Content $(CONFIG); \
		$$ip = ($$c | Select-String '^IP=').ToString().Split('=')[1]; \
		$$port = ($$c | Select-String '^PORT=').ToString().Split('=')[1]; \
	} else { \
		$$ip = '127.0.0.1'; \
		$$port = '53127'; \
	} \
	(Get-Content $(CLIENT_SRC)) -replace 'IP_PLACEHOLDER',$$ip -replace '10293847576',$$port | Set-Content $(CLIENT_TMP) \
	"
	$(CXX) $(CXXFLAGS) $(CLIENT_TMP) -o $(CLIENT_BIN) $(LDFLAGS)
	del /Q $(CLIENT_TMP)
	@echo Using IP=$$ip PORT=$$port

$(SERVER_BIN):
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) $(LDFLAGS)

clean:
	del /Q *.o 2>nul
	del /Q client_tmp.cpp 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: fclean all