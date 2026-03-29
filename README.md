# 🔌 Reverse Shell - Windows (C++)

A minimal TCP reverse shell implementation built in C++ using the Windows Winsock API. This project demonstrates low-level networking concepts, client-server communication, and basic remote interaction in a controlled environment.

---

## 🚀 Features

* TCP client/server architecture
* Winsock (Windows networking API)
* Real-time connection handling
* Clean and readable C++ code
* Colored terminal output (ANSI support)

---

## 📁 Project Structure

```
src/
├── client/
│   └── main.cpp
├── server/
│   └── main.cpp
```

---

## ⚙️ Build

Using MinGW / MSYS2:

```bash
make
```

---

## ▶️ Usage

### 1. Start the server

```bash
./server.exe
```

### 2. Run the client

```bash
./client.exe
```

Once connected, the server will accept the incoming connection from the client.

---

## 🧠 What I Learned

* Socket lifecycle: `socket → bind → listen → accept`
* Client connection: `socket → connect`
* Blocking vs non-blocking behavior
* Basic network debugging and testing
* Structuring a clean C++ networking project

---

## ⚠️ Disclaimer

This project is for educational purposes only. It is designed to understand networking and system-level programming concepts in a safe and controlled environment.

---

## 💡 Future Improvements

* Multi-client handling (`select()`)
* Command execution
* Data transmission (`send` / `recv`)
* Error handling improvements

---

## 📌 Notes

Antivirus software may flag this project due to its behavior. This is expected, as reverse shells share similarities with malware techniques.

---

## 🧑‍💻 Author

Built as part of a cybersecurity learning journey focused on low-level networking and offensive security fundamentals.
