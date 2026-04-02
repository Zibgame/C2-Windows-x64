# 🧠 C2-Windows-x64

A **Command & Control (C2) framework** written in C++ for Windows x64 systems.

This project simulates a real-world **offensive security infrastructure** used to remotely control multiple machines (agents) from a central server.

It is designed to demonstrate how attackers (or red teamers) build scalable systems to:

* maintain access to machines
* execute commands remotely
* manage multiple targets simultaneously

---

## 🚀 Features

* 🧠 Full C2 architecture (server ↔ agents)
* 🔗 TCP communication using Winsock (low-level networking)
* 👥 Multi-agent handling with `select()` (no threads per client)
* 🔐 Time-based authentication (anti-unauthorized access)
* 💻 Remote command execution via hidden processes
* 🪟 Stealth agent (no console / background execution)
* 📄 Logging system (server-side activity tracking)
* 🎨 ANSI CLI interface (operator feedback)

---

## 🎯 What this project demonstrates

This is NOT a simple reverse shell.

It implements core concepts found in real C2 frameworks:

* Agent ↔ Server communication protocol
* Authentication layer
* Multi-client orchestration
* Command execution pipeline
* Basic OPSEC considerations (stealth, timing, minimal footprint)

---

## 📁 Project Structure

```
src/
├── client/                # Agent (executed on target machine)
│   └── main.cpp
├── server/                # C2 server (operator side)
│   ├── main.cpp
│   ├── class/
│   │   ├── client.cpp
│   │   └── server.cpp
│   ├── cli/
│   │   └── cli.cpp       # Operator command interface
│   └── log/
│       └── log.cpp
```

---

## ⚙️ Build

Using MinGW / MSYS2:

```bash
mingw32-make re
```

### 🔧 Generated binaries

* `client.exe` → Agent (debug mode)
* `client_no_console.exe` → Agent (stealth mode)
* `server.exe` → C2 server

---

## ▶️ Usage

### 1. Start the C2 server

```bash
./server.exe
```

### 2. Execute the agent on a target machine

```bash
./client.exe
```

or stealth mode:

```bash
./client_no_console.exe
```

---

## 🧠 Architecture

### 🔌 Agent lifecycle

1. Connects to the C2 server
2. Sends authentication token
3. Sends hostname (target identification)
4. Waits for commands
5. Executes instructions and returns output

---

### 🔑 Authentication system

* Shared secret hardcoded
* Time-based rotation (every 30 seconds)
* Prevents unauthorized connections

---

### 💻 Remote command execution

* Commands sent from C2 server
* Executed using `CreateProcess`
* No visible window (`CREATE_NO_WINDOW`)
* Output captured via pipes
* Sent back to operator

---

### 👥 Multi-agent management

* Uses `select()` for scalability
* No thread-per-client overhead
* Tracks:

  * authentication state
  * hostname
  * connection time

---

### 📄 Logging

Logs stored in:

```
other/logs/log.txt
```

Example:

```
Succes auth
```

---

## 🧪 Example Operations

* Execute system commands remotely
* Identify active machines (hostname)
* Drop unauthenticated agents
* Blacklist specific IPs

---

## ⚠️ Disclaimer

This project is for **educational and authorized security research only**.

It is intended to demonstrate:

* how C2 infrastructures work
* how attackers manage remote systems
* how defensive teams can understand these mechanisms

Do NOT use on systems without explicit permission.

---

## 💡 Roadmap

* 🔒 TLS encryption (secure communications)
* 🧩 Modular command system (plugin-based tasks)
* 📊 Advanced logging (timestamps, agent IDs)
* 🔁 Auto-reconnect agents (persistence)
* 🌐 Cross-platform agents (Linux)
* 🧠 Task queue system (async execution)
* 📡 Advanced OPSEC features

---

## 🧑‍💻 Author

Built as part of a low-level systems and offensive security learning journey.

Focus: scalability, realism, and understanding of modern C2 design.
