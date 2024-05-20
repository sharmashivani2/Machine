#!/usr/bin/python
#-*- encoding:utf-8 -*-
import tornado.web
import tornado.ioloop
from tornado.ioloop import IOLoop, PeriodicCallback
import tornado.process
import tornado.template
import tornado.httpserver
import os,time
import serial
import subprocess
import binascii
# USB2.0-Ser!

from configparser import ConfigParser
os.chdir(os.path.dirname(os.path.realpath(__file__)))
configfile_name = "config.ini"
if not os.path.isfile(configfile_name):
    # Create the configuration file as it doesn't exist yet
    cfgfile = open(configfile_name, 'w')
    # Add content to the file
    Config = ConfigParser()
    Config.add_section('api')
    Config.set('api', 'port', '3015')
    Config.add_section('laser')
    Config.set('laser', 'desc', "'LOCATION=1-1'")
    Config.set('laser', 'baudrate', '9600')
    Config.set('laser', 'timeout', '1')
    Config.add_section('data')
    Config.set('data', 'ten_percent','9E 02 02 02 0C 0a 00 01 01 ae 3b 0d') # set 10%
    Config.set('data', 'twenty_percent','9e 02 02 02 0c 14 00 01 01 53 82 0d') # set 20%
    Config.set('data', 'thirty_percent','9e 02 02 02 0c 1e 00 01 01 f8 ea 0d') # set 30%
    Config.set('data', 'fourty_percent','9e 02 02 02 0c 28 00 01 01 88 e1 0d') # set 40%
    Config.set('data', 'fifty_percent','9e 02 02 02 0c 32 00 01 01 84 92 0d') # set 50%
    Config.set('data', 'sixty_percent','9e 02 02 02 0c 3c 00 01 01 de 30 0d') # set 60%
    Config.set('data', 'seventy_percent','9e 02 02 02 0c 46 00 01 01 00 1a 0d') # set 70%
    Config.set('data', 'eighty_percent','9e 02 02 02 0c 50 00 01 01 3e 26 0d') # set 80%
    Config.set('data', 'ninty_percent','9e 02 02 02 0c 5a 00 01 01 95 4e 0d') # set 90%
    Config.set('data', 'nintyfive_percent','9e 02 02 02 0c 5f 00 01 01 d0 f2 0d') #set 95%
    Config.set('data', 'hudered_percent','9E 02 02 02 0C 64 00 01 01 26 c0 0d') # set 100%
    Config.set('data', 'enabled', ' 9e 02 02 03 0c 01 00 00 00 01 43 0d ') # laser ENABLE
    Config.set('data', 'disabled', '9e 02 02 03 0c 00 00 00 00 b5 35 0d') # laser DISABLE
    Config.set('data', 'on', '9e 02 02 04 0c 01 00 00 00 40 8b 0d') #laser fire enable
    Config.set('data', 'off', '9e 02 02 04 0c 00 00 00 00 f4 fd 0d ') #laser fire disable
    Config.set('data', 'ae', '9e 02 02 05 0c 01 00 00 00 e0 ce 0d ') # aiming enable
    Config.set('data', 'ad', '9e 02 02 05 0c 00 00 00 00 54 b8 0d') # aiming disable
    Config.write(cfgfile)
    cfgfile.close()
configReader = ConfigParser()
configReader.read('config.ini')
baudrate = configReader['laser'].getint('baudrate')
sr_timeout = 1
api_port = configReader['api'].getint('port')
desc = configReader['laser']['desc']
proc = subprocess.Popen(['python -m serial.tools.list_ports '+desc], stdout=subprocess.PIPE, shell=True)
(port, err) = proc.communicate()
port = port.strip().decode('UTF-8')
print(port,baudrate)
sr=serial.Serial()
sr.port=str(port)
sr.baudrate=baudrate
sr.timeout=1
sr.stopbits=1
sr.bytesize=8
def chksm(dt):
    data = binascii.crc_hqx(dt,0)
    data1 = hex(data)[2:]
    print("data1",data1)
    crcl = data1[2:]
    crch = data1[:2]
    final = ' '+crcl+' '+crch+' 0d'
    print(final)
    return final
def dectohex(number):
    ns = str(number)
    dectohex_table={'0':'0', '1':'1', '2':'2', '3':'3', '4':'4', '5':'5', '6':'6', '7':'7', '8':'8', '9':'9', '10':'A', '11':'B', '12':'C', '13':'D', '14':'E', '15':'F','20':'14','25':'19','30':'1E','35':'23','40':'28','45':'2D','50':'32','55':'37','60':'3C','65':'41','70':'46','75':'4B','80':'50','85':'55','90':'5A','95':'5F','100':'64'}
    if ns in dectohex_table:
        nss = dectohex_table.get(ns)
        return nss
# def laser(data):
    # sr=serial.Serial()
    # sr.port=str(port)
    # sr.baudrate=baudrate
    # sr.timeout=1
    # sr.stopbits=1
    # sr.bytesize=8

#     sr.open()
#     if sr.is_open:
#         print("port is open")
#         sr.write(b'\x9e\x00\x00\x02\x0c\x00\x00\x00\x00\x50\x9f\x0d')
#         time.sleep(1.5)
#         response1 = sr.read()
#         response1 = binascii.hexlify(response1)
#         print(response1)
#         data=sr.write(data)
#         time.sleep(1.5)
#         response = sr.read()
#         response = binascii.hexlify(response)
#         # print("resp",str(response)
#         print(response)
#         print(data)
#         return response.decode('utf-8')
#     else:
#         print("port is not open")
#         return 0
class Laser(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')
class LaserStart(tornado.web.RequestHandler):
    def get(self):
        sr.open()
        if sr.is_open:
            print("port is open")
            sr.write(b'\x9e\x00\x00\x02\x0c\x00\x00\x00\x00\x50\x9f\x0d')
            time.sleep(1)     
            resp = sr.readline().decode('utf-8')
            print(resp)
            response1 = sr.read()
            response1 = binascii.hexlify(response1)
            print(response1)
            data=sr.write(data)
            time.sleep(1.5)
            response = sr.read()
            response = binascii.hexlify(response)
            # print("resp",str(response)
            print(response) 
            print(data)
            return response.decode('utf-8')
        else:
            print("check com port")
            # sr.read(resp)

class Enabled(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['enabled']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x03\x0c\x01\x00\x00\x00\x01\x43\x0d')     
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'ENABLED'})
            print(resp)
        else:
            print("check com port")
class Disabled(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['disabled']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x03\x0c\x00\x00\x00\x00\xb5\x35\x0d')     
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'DISABLED'})
            print(resp)
        else:
            print("check com port")
class LaserOn(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['off']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x04\x0c\x01\x00\x00\x00\x40\x8b\x0d')     
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'ON'})
            print(resp)
            # resp = laser(bytes.fromhex(data))
            # ps = int(self.get_argument("power"))
            # print(ps)
            # percent = dectohex(ps)
            # data1 = '9E 02 02 02 0C'
            # if len(percent) is 1:
            #     data2 = ' 0'+percent
            # else:
            #     data2 = ' '+percent
            # data3 = ' 00 01 01'
            # dt1 = data1+data2+data3
            # dt = bytes.fromhex(dt1)
            # fdata = dt1+chksm(dt)
            # fdata1 = bytes.fromhex(fdata) 
            # print('fd ',fdata1)
            # # print(bytes.fromhex(fdata1))
            # resp = laser(fdata1)
        else:
            print("check com port")
        
        
class LaserOff(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['off']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x04\x0c\x00\x00\x00\x00\xf4\xfd\x0d') #9e 02 02 04 0c 00 00 00 00 f4 fd 0d 
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'OFF'})
            print(resp)
        else:
            print("check com port")

class Aiming_Enable(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['ae']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x05\x0c\x01\x00\x00\x00\xe0\xce\x0d')#9e 02 02 05 0c 01 00 00 00 e0 ce 0d     
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'AIMING_ENABLE'})
            print(resp)
        else:
            print("check com port")

class Aiming_Disable(tornado.web.RequestHandler):
    def get(self):
        # data = configReader['data']['ad']
        if sr.is_open:
            sr.write(b'\x9e\x02\x02\x05\x0c\x00\x00\x00\x00\x54\xb8\x0d') #9e 02 02 05 0c 00 00 00 00 54 b8 0d    
            time.sleep(1)
            resp = sr.readline().decode('utf-8')
            print(resp)
            resp = laser(bytes.fromhex(data))
            self.write({'Laser': 'AIMING_DISABLE'})
            print(resp)
        else:
            print("check com port")

# class close(tornado.web.RequestHandler):
#     def get(self):
#         sr.close()
# class open1(tornado.web.RequestHandler):
#     def get(self):
#         sr.open()
def make_app():
    return tornado.web.Application([("/", Laser),("/start", LaserStart),("/enabled", Enabled),("/disabled", Disabled),("/on", LaserOn),("/off", LaserOff),("/ae", Aiming_Enable), ("/ad", Aiming_Disable)],template_path=os.path.join(os.path.dirname(__file__), "templates"))

if __name__ == '__main__':
    app = make_app()
    app.listen(api_port)
    print("laser is listening for commands on port: "+str(api_port))
    IOLoop.instance().start()
