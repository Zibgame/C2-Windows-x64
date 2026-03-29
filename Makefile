CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -static -static-libgcc -static-libstdc++ \
-Isrc/server/class \
-Isrc/server/cli \
-Isrc/server/log
LDFLAGS = -lws2_32 -lkernel32

CONFIG = config.txt

CLIENT_SRC = src/client/main.cpp
CLIENT_TMP = client_tmp.cpp
CLIENT_BIN = client.exe
CLIENT_NO_CONSOLE_BIN = client_no_console.exe

SERVER_SRC = src/server/main.cpp \
             src/server/class/client.cpp \
             src/server/class/server.cpp \
             src/server/cli/cli.cpp \
             src/server/log/log.cpp

SERVER_BIN = server.exe

all: $(CLIENT_BIN) $(CLIENT_NO_CONSOLE_BIN) $(SERVER_BIN)

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
	@echo Built $(CLIENT_BIN)

$(CLIENT_NO_CONSOLE_BIN):
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
	$(CXX) $(CXXFLAGS) $(CLIENT_TMP) -o $(CLIENT_NO_CONSOLE_BIN) $(LDFLAGS) -mwindows
	del /Q $(CLIENT_TMP)
	@echo Built $(CLIENT_NO_CONSOLE_BIN)

$(SERVER_BIN):
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) $(LDFLAGS)
	@echo Built $(SERVER_BIN)

client: $(CLIENT_BIN)
client_nc: $(CLIENT_NO_CONSOLE_BIN)
server: $(SERVER_BIN)

clean:
	del /Q *.o 2>nul
	del /Q client_tmp.cpp 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: fclean all