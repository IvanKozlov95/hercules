#!/nfs/2017/i/ikozlov/.brew/bin/python3

import os
import sys
from util import *
from languages.C import C

if len(sys.argv) < 3:
	print("usage: python start.py [project name] [project language] [-p path]")
	sys.exit()

name = sys.argv[1]
lang = sys.argv[2].split(' ')[0].title()
path = os.getcwd()
if len(sys.argv) >= 4:
	if sys.argv[3] == '-p':
		if len(sys.argv) is not 5:
			print("specify the path")
			sys.exit()
		path = sys.argv[4]
		if len(path) >= 2 and  path[0] is '.' and path[1] is '/':
			path = os.getcwd() + path[1:]
		if len(path) == 2 and path[0] is not '.' and path[1] is not '/':
			print("use relative paths like ./foder or ~/folder")
			sys.exit()
		elif path[0] is not '/' and path[0] is not '.':
			print("use relative paths like ./ or ~/folder")
			sys.exit()	
		elif len(path) is 1 and path[0] is '/':
			print("don't create a project in root. This is bad.")
			sys.exit()
	else:
		print("unknown command " + sys.argv[3])
path = path if path[-1] == '/' else path + '/'
gitignore = path + name + '/.gitignore'
languages = {
	'C': C
}

def download_gitignore():
	link = 'https://raw.githubusercontent.com/github/gitignore/master/' + lang + '.gitignore'
	os.system('curl -s ' + link + ' --output .gitignore')
	file = open(gitignore)
	if '404' in file.read():
		os.remove(gitignore)
		msg('red', 'Couldn\'t find gitignore for your language, sorry.')

def mkdir():
	if not os.path.exists(path + name):
		os.makedirs(path + name)
		msg('green', 'Created directory ' + path)
	else:
		msg('yellow', 'Directory ' + path + name + ' already exists.')
		onno = lambda: (
			msg('red', 'Aborting...'),
			sys.exit()
		)
		ask_until_yn('Continue? (y/n)?\n', lambda: os.system('git init'), onno)

def create_project():
	mkdir()
	os.chdir(path + name)
	download_gitignore()
	nolang = lambda: msg('red', 'Your language isn\'t in my database')
	languages[lang](path + name, name) if lang in languages.keys() else nolang()

create_project()