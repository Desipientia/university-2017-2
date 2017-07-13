import socket
import sys
import argparse

HOST, PORT = 'localhost', 8080

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

argsParser = argparse.ArgumentParser()
argsParser.add_argument('-c', dest='command', required=True, type=str)
args = argsParser.parse_args()

if (args.command == 'run'):
    s.send(b'r')
elif (args.command == 'stop'):
    s.send(b's')
else:
    s.send(b'o')

print('answer:', str(s.recv(1000)))