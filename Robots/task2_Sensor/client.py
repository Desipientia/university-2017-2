import socket
import sys
import argparse
import laserscan_pb2 as proto
import time
import threading

HOST, PORT = 'localhost', 8080

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

isExit = False

laserScans = []

from pynput import keyboard

def on_press(key):
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
    except AttributeError:
        if str(key) == 'Key.up':
            print('up')
            s.send(b'f')
        elif str(key) == 'Key.down':
            print('down')
            s.send(b's')
        elif str(key) == 'Key.left':
            print('left')
            s.send(b'l')
        elif str(key) == 'Key.right':
            s.send(b'r')


def threadFunc(s):
    while True:
        if (isExit):
            break
        laserScan = proto.LaserScan()
        laserScan.ParseFromString(s.recv(11688))
        laserScans.append(laserScan)
        print('length of laserScans =', len(laserScans))
        time.sleep(0.1)

thread = threading.Thread(target=threadFunc, args=(s,))

thread.start()

def on_release(key):
    if key == keyboard.Key.esc:
        s.send(b'q')
        isExit = True
        return False

# Collect events until released
with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
            listener.join()
