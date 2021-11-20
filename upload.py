import serial
from xmodem import XMODEM as xm 
from sys import stdout

ser = serial.Serial(port='/dev/ttyUSB1',baudrate=500000,timeout=1)

def getc(size, timeout=1):   
    recv = ser.read(size)
    # if not recv is None:
    #     stdout.write("<"+recv.decode())
    return recv or None

def putc(data, timeout=1):
    #print("xm send {} bytes".format(len(data)))
    #stdout.write(data.decode())
    return ser.write(data)

packets = 0
errors = 0


def xm_callback(total_packets, success_count, error_count):
    global packets,errors
    stdout.write(".")
    stdout.flush()
    packets = total_packets
    errors = error_count

def transfer_end():
    stdout.write("]\n")
    print("total packets: {}  error packets: {}".format(packets,errors))    


modem = xm(getc,putc)

f = open(".pio/build/Arty/firmware.bin","rb")
ser.write(b"i\r")
response=ser.read_until(b">\r\n")
print(response)

ser.write(b"x\r")
response=ser.read_until(b"\0x15")
print(response)
print("Sending data")
stdout.write("[")
if modem.send(f,callback=xm_callback):
    transfer_end()
    print("XMODEM transfer successfull, starting program")
    ser.write(b"g\r")
else:
    transfer_end()
    print("XMODEM Transfer to board failed")

