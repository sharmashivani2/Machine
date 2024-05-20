#!/usr/bin/python
#-*- encoding:utf-8 -*-
import tornado.web
import tornado.ioloop
from tornado.ioloop import IOLoop, PeriodicCallback
import tornado.process
import tornado.template
import tornado.httpserver
import json
import sys
import os,time
import socket
import requests
from configparser import ConfigParser
os.chdir(os.path.dirname(os.path.realpath(__file__)))
configfile_name = "config.ini"
if not os.path.isfile(configfile_name):
    # Create the configuration file as it doesn't exist yet
	cfgfile = open(configfile_name, 'w')
	# Add content to the file
	Config = ConfigParser()
	Config.add_section('api')
	Config.set('api', 'port', '3020')
	Config.add_section('laser')
	Config.set('laser', 'host', '192.168.1.102')
	Config.set('laser', 'port', '80')
	Config.write(cfgfile)
	cfgfile.close()
configReader = ConfigParser()
configReader.read('config.ini')
port = configReader['laser'].getint('port')
host = configReader['laser']['host']
api_port = configReader['api'].getint('port')

class Laser(tornado.web.RequestHandler):
	def get(self):
		self.render('index.html')
class LaserRemotekeyOn(tornado.web.RequestHandler):
    def get(self):
        requests.get('http://192.168.1.102/remote_key_on')
class LaserRemotekeyOff(tornado.web.RequestHandler):
    def get(self):
        requests.get('http://192.168.1.102/remote_key_off')
class LaserStartbuttonOn(tornado.web.RequestHandler):
    def get(self):
        requests.get('http://192.168.1.102/start_button_on')
class LaserStartbuttonOff(tornado.web.RequestHandler):
    def get(self):
        requests.get('http://192.168.1.102/start_button_off')
class sp(tornado.web.RequestHandler):
    def get(self):
        pw = int(self.get_argument("pw"))
        requests.get('http://192.168.1.177/pd'+str(pw))
class LaserOn(tornado.web.RequestHandler):
	def get(self):
		try:
			ft = int(self.get_argument("tt"))
			print(ft)
		except:
			ft = 0
		if(ft != 0):
			requests.get('http://192.168.1.102/Laser_on')
			time.sleep(ft)
			requests.get('http://192.168.1.102/Laser_off')
		else:
			requests.get('http://192.168.1.102/Laser_on')
class LaserOff(tornado.web.RequestHandler):
	def get(self):
		requests.get('http://192.168.1.102/Laser_off')
def make_app():
	return tornado.web.Application([("/", Laser),("/ron", LaserRemotekeyOn),("/roff", LaserRemotekeyOff),("/on", LaserOn),("/off", LaserOff),("/sp", sp)],template_path=os.path.join(os.path.dirname(__file__), "templates"))

if __name__ == '__main__':
	app = make_app()
	app.listen(api_port)
	print("Laser tcp is listening for commands on port:"+ str(api_port))
	IOLoop.instance().start()
