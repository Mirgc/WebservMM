#!/usr/bin/python

import time

# Prints out http headers
print("HTTP/1.1 200 OK\r")
print("Content-Type: text/html\r")
print("\r")

print("<!DOCTYPE html><html lang=\"en\"><body>")
print("<h1> Waiting forever!!! </h1>")

time.sleep(1000)

print("</body></html>")