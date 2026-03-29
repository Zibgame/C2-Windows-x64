# 🔌 Reverse Shell - Windows (C++)

A clean and modular TCP reverse shell built in C++ using the Windows Winsock API. This project focuses on low-level networking, process management, and multi-client handling in a controlled environment.

---

## 🚀 Features

* 🔗 TCP client/server architecture
* 🧠 Winsock (Windows networking API)
* 👥 Multi-client handling with `select()`
* 🔐 Time-based authentication system
* 💻 Remote command execution (hidden process)
* 🪟 No-console client build (stealth mode)
* 📄 File logging system
* 🎨 ANSI colored output

---

## 📁 Project Structure

```
src/
├── client/
│   └── main.cpp
├── server/
│   ├── main.cpp
│   ├── class/
│   │   ├── client.cpp
│   │   └── server.cpp
│   ├── cli/
│   │   └── cli.cpp
│   └── log/
│       └── log.cpp
```

---

## ⚙️ Build

Using MinGW / MSYS2:

```bash
make
```

### 🔧 Generated binaries

* `client.exe` → Debug version (console visible)
* `client_no_console.exe` → Stealth version (no console)
* `server.exe` → Multi-client server

---

## ▶️ Usage

### 1. Start the server

```bash
./server.exe
```

### 2. Run a client

```bash
./client.exe
```

or (stealth mode):

```bash
./client_no_console.exe
```

---

## 🧠 How it works

### 🔌 Connection flow

* Client connects to server
* Sends authentication token
* Sends hostname
* Waits for commands

### 🔑 Authentication

* Based on a shared secret
* Time-synchronized (rotates every 30 seconds)
* Prevents unauthorized connections

### 💻 Command execution

* Server sends command
* Client executes via `CreateProcess`
* No visible window (`CREATE_NO_WINDOW`)
* Output is captured via pipes and sent back

### 👥 Multi-client handling

* Server uses `select()`
* Handles multiple clients simultaneously
* Tracks authentication + hostname

### 📄 Logging

* Logs stored in:

```
other/logs/log.txt
```

* Example:

```
Succes auth
```

---

## 🧪 Useful Commands

### Kill all stealth clients

```bash
taskkill /IM client_no_console.exe /F
```

---

## ⚠️ Disclaimer

This project is strictly for educational purposes.

It demonstrates:

* Networking fundamentals
* Process management
* Client/server architecture

Do not use this on systems you do not own or have explicit permission to test.

---

## 💡 Future Improvements

* 🔒 Real encryption (TLS)
* 📊 Better logging system (timestamps, client IDs)
* 🧩 Modular command system
* 🌐 Cross-platform support (Linux client)
* 📡 Reconnection mechanism

---

## 🧑‍💻 Author

Built as part of a cybersecurity and low-level programming learning journey.

Focus: performance, clarity, and real-world understanding of system internals.
