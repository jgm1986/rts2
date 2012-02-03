#!/usr/bin/python

# Guiding script.
# (C) 2010 Martin Jelinek & Petr Kubanek
#
# rts2comm.py is included in RTS2 distribution. You must eithert copy it to the
# same location as this script, or include it in PYTHONPATH.
#
# As with all scripts intended to be called by RTS2 exe script command, you can
# test this script by calling it and verifiing that it prints something what
# does not look like error message on standard output.

import sys
import subprocess
import os
import rts2comm
import sextractor

class GuideScript (rts2comm.Rts2Comm):
	"""Guiding script."""
	def __init__(self):
		self.sextractor = sextractor.Sextractor(None)
		# size of big window - taken at the beginning of guiding to find bright star for guiding
		self.big_x = 324
		self.big_y = 69
		self.big_w = 1210
		self.big_h = 955

		# exposure time
		self.exptime = 2

		# size of small guiding subwindow
		self.w = 30
		self.h = 30

		# offsets bellow this size will be ignored
		self.x_sensitivity = 0.15
		self.y_sensitivity = 0.15
	
		# how much of the detected offset to apply (to dump resonance)
		self.ra_aggresivity = 0.7
		self.dec_aggresivity = 0.7

	def doGuiding(self,x,y):
		"""Guide the star on position x,y."""
		self.setValue('SHUTTER','LIGHT')

		winfmt="%d %d %d %d" % (x-int(self.w / 2),y-int(self.h / 2),self.w,self.h)
		self.log('I','guiding in CCD window ' + winfmt)
		self.setValue('WINDOW',winfmt)
		self.setValue('center_box',True)
			
		tar_SNR=10;  # target star errorbar in magnitude (for exposure optimization)

		current = self.getValueInteger('current','EXEC')

		while (True):
			next = self.getValueInteger('next','EXEC')

			# end if there is next target different from the current one
			if (next != current and next != -1):
				self.setValue('SHUTTER','LIGHT')
				return
				
			self.setValue('exposure',self.exptime) 
			image = self.exposure()

			# now get star center from camera driver
			x = self.getValueFloat('center_X')
			y = self.getValueFloat('center_Y')

			if (abs(x - 15) < self.x_sensitivity and abs (y - 15) < self.y_sensitivity):
				self.log('I','autoguiding below sensitivity %f %f' % (x,y))
				self.delete(image)
				continue

			ch_ra = float(change[0]) * self.ra_aggresivity
			ch_dec = float(change[1]) * self.dec_aggresivity

			self.log('I','guiding * center %f %f change %.1f %.1f (%f %f)' % (x,y,ch_ra*3600,ch_dec*3600,ch_ra,ch_dec))
			self.incrementValueType(rts2comm.TELESCOPE,'OFFS','%f %f' % (ch_ra, ch_dec))
			# os.system ('cat %s | su petr -c "xpaset ds9 fits"' % (image))
			self.delete(image)

	def run(self):
		# First we should do target centering
		## but at this moment let's omit that phase
		# And when it is centered, do the guiding itself		

		# 1536x1024 => 324 69 1210 955 (50% of the chip)
		self.setValue('WINDOW','%d %d %d %d' % (self.big_x, self.big_y, self.big_w, self.big_h))
		# make sure we are taking light images..
		self.setValue('SHUTTER','LIGHT')
				
		self.setValue('exposure',self.exptime)

		self.setValueType(rts2comm.TELESCOPE,'OFFS','0 0')

		image = self.exposure()

		self.sextractor.runSExtractor(image)
		x = int(float(values[0])) + self.big_x
		y = int(float(values[1])) + self.big_y
		self.log('I','values for autoguiding %d %d' % (x, y))
				
		# while we are current target
		self.doGuiding(x,y)

a = GuideScript()
a.run()
