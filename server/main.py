import socket

"""This is the main server class"""
class Server(object):
    SERVER_LISTEN_PORT = "0.0.0.0"
    SERVER_MAIN_PORT = 61110
    BUFFER_SIZE = 1024

    """This function initiates the server object"""
    def __init__(self):
        # Create a listening port of TCP/IP type
        self.listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Bind the listen socket to listen to all traffic on 61110 port
        self.listen_socket.bind((self.SERVER_LISTEN_PORT, self.SERVER_MAIN_PORT))

    """This function starts the server by listening to incoming clients"""
    def start(self):
        # Start listening to incoming connections with 1 client in queue
        self.listen_socket.listen(1)
        
        # Main loop of the server
        while True:
            # Accept of a connection of a client
            conn_socket, addr = self.listen_socket.accept()