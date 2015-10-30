import numpy as np

class PID():

	def __init__(self):

		self.k = [0,0,0]
		self.target = 0
		self.c_data = 0

	def run(self):
		print self.k

	def output(self):
		print self.output