# ft_irc - Internet Relay Chat Server

Welcome to **ft_irc**, a project where we built our own **IRC server** using **C++98**. This project will helped us understand **networking, sockets, and real-time communication** protocols.

## 📌 Features
- 🖥 **Multi-client handling**: Supports multiple concurrent connections.
- 🔗 **TCP/IP communication**: Works over IPv4/IPv6.
- 🔑 **Authentication system**: Clients must authenticate with a password.
- 📢 **Channel management**: Join, leave, and moderate chat rooms.
- 🛠 **Operator commands**: Kick, invite, topic, and mode settings.

---
## 📂 Project Structure
```plaintext
├── src/          # Source files
├── includes/     # Header files
├── Makefile      # Compilation script
└── README.md     # Project documentation
```
---
## ⚡ Installation & Execution
1. Clone the repository:
   ```sh
   git clone https://github.com/Tanezka/ft_irc.git
   cd ft_irc
   ```
2. Compile the project:
   ```sh
   make
   ```
3. Run the server:
   ```sh
   ./ircserv <port> <password>
   ```
   **Arguments:**
   - `port`: The port number the server listens on.
   - `password`: The password required for client connections.
4. Connect to the Server:
   - Use Hexchat to connect server's ip.

---
## 📜 Server Mechanics
1. **Client Authentication**
   - Users must provide a password to connect.
2. **Channel Operations**
   - Clients can join and leave channels.
   - Messages sent in a channel are relayed to all members.
3. **Operator Privileges**
   - `KICK`: Remove a user from a channel.
   - `INVITE`: Invite a user to a private channel.
   - `TOPIC`: Set or view the channel topic.
   - `MODE`: Modify channel settings (`i`, `t`, `k`, `o`, `l`).
4. **TCP Communication**
   - Handles socket connections with non-blocking I/O.

---
## 📢 Contributions
Contributions are welcome! Feel free to submit issues or pull requests.

---
## 📄 License
This project is licensed under the **MIT License**.

💬 *Happy coding and enjoy building your own IRC server!*
