#!/usr/bin/python

import locale
import string
import sys
from optparse import OptionParser

def readFile(fileName, parser):
	try:
		f = open(fileName, 'r')
		lines = f.read().split('\n')
		f.close()
		return lines
	except IOError as e:
		errno = e.errno
		strerror = e.strerror
		parser.error("{0}". format(strerror))

def checkSorted(f):
	for i in range(len(f)-1):
		if(locale.strcoll(f[i],f[i+1]) > 0):
			return False
	return True

def commSorted(f1, f2, options):
	f1Sorted = checkSorted(f1)
	f2Sorted = checkSorted(f2)
	if(f1Sorted == False or f2Sorted == False):
		if(f1Sorted == False):
			print("File 1 is not sorted")
		if(f2Sorted == False):
			print("File 2 is not sorted")
		exit()

	one = two = three = ""

	if(options.one == False):
		one = ' '*(8)
	if(options.two == False):
		two = ' '*(8)
	if(options.three == False):
		three = ' '*(8)

	i = 0
	j = 0
	while(i != len(f1) and j != len(f2)):
		line = ""
		if(f1[i] == f2[j]):
			if(options.three == False):
				print(one+two+f1[i])
			i+=1
			j+=1
		elif(f1[i] < f2[j]):
			if(options.one == False):
				print(f1[i])
			i+=1
		elif(f1[i] > f2[j]):
			if(options.two == False):
				print(one+f2[j])
			j+=1
	if(i != len(f1)):
		line = ""
		for n in range(i, len(f1)):
			if(options.one == False):
				print(f1[i])
	if(j != len(f2)):
		line = ""
		for n in range(j, len(f2)):
			if(options.two == False):
				print(one+f2[j])

def commUnsorted(f1, f2, options):

	one = two = three = ""

	if(options.one == False):
		one = ' '*(8)
	if(options.two == False):
		two = ' '*(8)
	if(options.three == False):
		three = ' '*(8)	

	for i in range(len(f1)):
		for j in range(len(f2)):
			if(f1[i] == f2[j]):
				if(options.three == False):
					print(one+two+f1[i])
				f2[j] = None
				break
			elif(j == len(f2) - 1):
				if(options.one == False):
					print(f1[i])
	for i in range(len(f2)):
		if(f2[i] != None):
			if(options.two == False):
				print(one+f2[i])

def main():
	locale.setlocale(locale.LC_ALL, '')

	version_msg = "%prog 1.0"
	usage_msg = """%prog [OPTION]... FILE1 FILE2
	Compare FILE1 and FILE2 line by line."""

	parser = OptionParser(version=version_msg, usage=usage_msg)

	parser.add_option("-1",
		action="store_true", dest="one", default=False,
		help="suppress column 1 (lines unique to FILE1)")
	parser.add_option("-2",
		action="store_true", dest="two", default=False,
		help="suppress column 2 (lines unique to FILE2)")
	parser.add_option("-3",
		action="store_true", dest="three", default=False,
		help="suppress column 3 (lines that appear in both files)")
	parser.add_option("-u",
		action="store_true", dest="u", default=False,
		help="causes comm to work even if its inputs are not sorted")
	options, args = parser.parse_args(sys.argv[1:])

	if len(args) != 2:
		parser.error("Wrong number of operands")

	if(args[0] == '-'):
		f1 = sys.stdin.read().split('\n')
	else:
		f1 = readFile(args[0], parser)
	f1 = f1[:-1]

	if(args[1] == '-'):
		f2 = sys.stdin.read().split('\n')
	else:
		f2 = readFile(args[1], parser)
	f2 = f2[:-1]
	
	if options.u:
		lines = commUnsorted(f1, f2, options)
	else:
		lines = commSorted(f1, f2, options)

if __name__ == "__main__":
	main()
