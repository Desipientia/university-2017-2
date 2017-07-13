import socket
import sys
import argparse

HOST, PORT = 'localhost', 9090

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

from pynput import keyboard

def on_press(key):
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
    except AttributeError:
        if key == keyboard.Key.shift:
            print('up')
            s.send(b'f')
        elif key == keyboard.Key.cmd:
            print('down')
            s.send(b's')
        elif key == keyboard.Key.alt:
            print('left')
            s.send(b'l')
        elif key == keyboard.Key.alt_r:
            print('right')
            s.send(b'r')

def on_release(key):
    if key == keyboard.Key.esc:
        s.send(b'q')
        return False

with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
