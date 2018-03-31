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
if len(sys.argv) is 5:
	if sys.argv[3] == '-p':
		path = sys.argv[4]
path = path if path[-1] == '/' else path + '/'
print(name)
print(path)
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
		msg('yellow', 'Directory ' + path + ' already exists.')
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