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
    Config.set('laser', 'host', '192.168.1.253')
    Config.set('laser', 'port', '1045')
    # Config.set('laser', 'timeout', '1')
    Config.write(cfgfile)
    cfgfile.close()
configReader = ConfigParser()
configReader.read('config.ini')
port = configReader['laser'].getint('port')
host = configReader['laser']['host']
api_port = configReader['api'].getint('port')
try:
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error as e:
    print("error in socket creation")
# print(port,baudrate)
def laser(data):
    server.send(data,(host,port))
def chksm(dt):
    data = binascii.crc_hqx(dt,0)
    data1 = hex(data)[2:]
    print("data1",data1)
    crcl = data1[2:]
    crch = data1[:2]
    final = ' '+crcl+' '+crch+' 0d'
    print(final)
    return final
class Laser(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')
class Enabled(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 03 0C 01 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata)
        resp = laser(fdata1) 
        self.write({'Laser': 'ENABLED'})
        print(resp)
class Disabled(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 03 0C 00 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print("dt1 ",fdata)
        resp = laser(fdata1) 
        self.write({'Laser': 'DISABLED'})
class Lasersp(tornado.web.RequestHandler):
    def get(self):
        ps = int(self.get_argument("power"))
        print(ps)
        percent = hex(ps)[2:]
        data1 = '9E 02 02 02 0C'
        if len(percent) is 1:
            data2 = ' 0'+percent
            print(data2)
        else:
            data2 = ' '+percent
            print(data2)
        data3 = ' 00 01 01'
        dt1 = data1+data2+data3
        dt = bytes.fromhex(dt1)
        fdata = dt1+chksm(dt)
        print("dt1 ",fdata)
        fdata1 = bytes.fromhex(fdata) 
        # print(bytes.fromhex(fdata1))
        resp = laser(fdata1)
class LaserOn(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 04 0C 01 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata)
        resp = laser(fdata1)      
class LaserOff(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 04 0C 00 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata)
        resp = laser(fdata1) 
        self.write({'Laser': 'OFF'})
class Aiming_Enable(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 05 0C 01 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata)
        resp = laser(fdata1) 
        self.write({'Laser': 'AIMING_ENABLE'})
class Aiming_Disable(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 02 02 05 0C 00 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata) 
        resp = laser(fdata1) 
        self.write({'Laser': 'AIMING_DISABLE'})
class Alarm(tornado.web.RequestHandler):
    def get(self):
        data1 = '9E 00 00 02 0C 00 00 00 00'
        dt = bytes.fromhex(data1)
        fdata = data1+chksm(dt)
        fdata1 = bytes.fromhex(fdata) 
        print('fd ',fdata) 
        resp = laser(fdata1) 
        self.write({'Laser': 'ALARM'})
class close(tornado.web.RequestHandler):
    def get(self):
        sr.close()
class open1(tornado.web.RequestHandler):
    def get(self):
        sr.open()
def make_app():
    return tornado.web.Application([("/", Laser),("/enabled", Enabled),("/disabled", Disabled),("/lsp", Lasersp),("/lon", LaserOn),("/off", LaserOff),("/ae", Aiming_Enable), ("/ad", Aiming_Disable),("/al",Alarm)],template_path=os.path.join(os.path.dirname(__file__), "templates"))

if __name__ == '__main__':
    app = make_app()
    app.listen(api_port)
    print("laser is listening for commands on port: "+str(api_port))
    IOLoop.instance().start()
