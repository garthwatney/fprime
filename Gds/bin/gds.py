#!/usr/bin/env python
# encoding: utf-8
'''
This is the f prime ground support equipment client
program.  It is designed to bring up a front end
GUI sending commands and receiving telemetry that
are defined utilizing the various XML files
used by the Autocoder to generate framework codes.

@author:     Leonard J. Reder

@copyright:  2014 California Institute of Technology. All rights reserved.

@license:    license

@contact:    reder@jpl.nasa.gov
'''
import sys
import os
import time
import glob
import random
from utils import PortFinder


import traceback

from distributor import distributor
from client_socket import client_socket
from decoders import event_decoder
from decoders import ch_decoder
from loaders import event_py_loader
from loaders import cmd_py_loader
from encoders import cmd_encoder
from loaders import ch_py_loader


from pprint import pprint

import wx
from gui import GDSMainFrameImpl
from optparse import OptionParser

from time import sleep

__all__ = []
__version__ = 0.1
__date__ = '2015-04-03'
__updated__ = '2015-04-03'
LOGGER = None

def connect(opts):
	"""
	Routine to launch Threaded TCP Server
	and connect to gse.py.  The routine
	creates default log files of stdout
	and stderr.
	"""
	main_panel.TopPanel(None, None, opts).startTCP()


def execute(opts):
	"""
	Routine to launch a FSW binary that
	must be specified on the command line.
	The routine creates default log files
	of stdout and stderr.
	"""
	#
	# Setup log file name here...
	#
	if opts == None:
		p = os.environ['HOME'] + os.sep + 'fprime_logs' + os.sep + "app"
	else:
		p = opts.log_file_path + os.sep + "app"
	#
	if not os.path.exists(p):
		os.makedirs(p)
	#
	logfile = p + os.sep + time.strftime("%y%m%d%H%M%S", time.gmtime()) + '_GSE.log'
	#
	# Launch the FSW application saving both stdout and stdin to common log.
	#
	op_sys = os.uname()[0]
	if 'BUILD_ROOT' in os.environ:
		build_root_path = os.environ['BUILD_ROOT']
	else:
		print "EXCEPTIONS:  BUILD_ROOT Environment Variable not found!"
		raise exceptions.EnvironmentError

	app_cmd    = "\"%s -p %s | tee -a %s; exit\"" % (opts.exec_app, opts.port, logfile)
	#app_cmd    = "\"%s |& tee -a %s; exit\"" % (opts.exec_app, logfile)
	#app_cmd    = "\"%s\"" % (opts.exec_app)
	if op_sys == 'Darwin':
		app_script = '/usr/bin/osascript '
		app_args   = '-e \'tell application \"Terminal" to do script '
		# Note the exit is so Mac Terminal.app window will exit when process killed...
		cmd = app_script + app_args + app_cmd +  " \'"
	else:
		app_script = "/usr/bin/gnome-terminal"
		#app_args   = "-e "
		app_args   = " -e"
		cmd = app_script + app_args + app_cmd
	os.system(cmd)
	LOGGER.info("FSW Application Started: %s" % opts.exec_app)


def main_window_start():
	"""
	Everything GUI gets started from this routine.
	"""
	app = wx.App(False)
	frame = GDSMainFrameImpl.MainFrameImpl(None)
	frame.Show(True)

	app.MainLoop()
	return frame


def main(argv=None):
	'''
	Command line options.
	'''
	program_name = os.path.basename(sys.argv[0])
	program_version = "v0.1"
	program_build_date = "%s" % __updated__

	program_version_string = '%%prog %s (%s)' % (program_version, program_build_date)
	#program_usage = '''usage: spam two eggs''' # optional - will be autogenerated by optparse
	program_longdesc = '''''' # optional - give further explanation about what the program does
	program_license = "Copyright 2015 user_name (California Institute of Technology)                                            \
				ALL RIGHTS RESERVED. U.S. Government Sponsorship acknowledged."
	#
	# setup option parser
	parser = OptionParser(version=program_version_string, epilog=program_longdesc, description=program_license)
	parser.add_option("-d", "--dictionary", dest="generated_path", action="store", type="string", \
						help="Set base path to generated command/telemetry definition files [default: %default]")
	parser.add_option("-a", "--addr", dest="addr", action="store", type="string", help="set threaded tcp socket server address [default: %default]", \
						default="127.0.0.1")
	parser.add_option("-c", "--connect", action="store_true", dest="connect", help="Launches the Threaded TCP Socket Server on startup and connect to it [default: %default]", \
						default=False)
	parser.add_option("-p", "--port", dest="port", action="store", type="int", help="Set threaded tcp socket server port [default: %default]", \
						default=50007)
						#default=PortFinder.old_getport(50000,[]))

	parser.add_option("-e", "--execute", dest="exec_app", action="store", type="string", help="Execute the specified fsw application after socket server and UI are up [default: %default]")
	parser.add_option("-L", "--log-file-path", dest="log_file_path", action="store", type="string", help="Path to log files [default: %default]")
	parser.add_option("-r", "--log-file-prefix", dest="log_file_prefix", action="store", type="string", help="Prefix in log file path for each run. [default: current date/time]", \
						default=None)
	parser.add_option("-l", "--log-time", dest="log_time", action="store", type="string", help="Time used for logging. (local, GMT)", \
						default="local")
	parser.add_option("-n", "--no-about", dest="no_about", action="store_true", help="Do not show about text screen on start", \
						default=True)

	# process options
	(opts, args) = parser.parse_args(sys.argv)
	pprint(opts)

	app = wx.App(False)

	distrib = distributor.Distributor()
	cli = client_socket.ThreadedTCPSocketClient()

	eldr = event_py_loader.EventPyLoader()
	eid_dict = eldr.get_id_dict(opts.generated_path + os.sep + "events")

	cldr = cmd_py_loader.CmdPyLoader()
	cname_dict = cldr.get_name_dict(opts.generated_path + os.sep + "commands")

	ch_ldr = ch_py_loader.ChPyLoader()
	ch_dict = ch_ldr.get_id_dict(opts.generated_path + os.sep + "channels")

	frame = GDSMainFrameImpl.MainFrameImpl(None, cname_dict)

	dec = event_decoder.EventDecoder(eid_dict)
	dec.register(frame.event_pnl)

	cmd_enc = cmd_encoder.CmdEncoder(cname_dict)

	frame.cmd_pnl.register_encoder(cmd_enc)

	cmd_enc.register(cli)

	
	distrib.register("FW_PACKET_LOG", dec)

	ch_dec = ch_decoder.ChDecoder(ch_dict)
	ch_dec.register(frame.telem_pnl)
	distrib.register("FW_PACKET_TELEM", ch_dec)


	pprint(opts)

	cli.register_distributor(distrib)
	sleep(1)
	cli.connect(opts.addr, opts.port)
	sleep(1)
	cli.send("Register GUI\n")



	frame.Show(True)

	app.MainLoop()
	cli.disconnect()

if __name__ == "__main__":
	sys.exit(main())
