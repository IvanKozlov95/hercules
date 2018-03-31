import os
import sys

colors = {
	'red': '\033[0;31m',
	'green': '\033[92m',
	'end': '\033[0m',
	'white': '\033[97m',
	'yellow': '\033[93m'
}

def msg(color, msg):
	print(colors[color] + msg + colors['end'])

def ask_until_yn(question, yes, no):
	yn = input(question)
	while (yn != 'y'):
		if yn in 'nN':
			no() if no is not None else None
			return
		else:
			msg('white', 'Please answer yY or nN.')
		yn = input(question)
	yes() if yes is not None else None