import os
import sys
from util import *
from languages.C import C

if len(sys.argv) < 2:
	print("usage: python start.py [project name] [project language]")
	sys.exit()

name = sys.argv[1]
lang = sys.argv[2].split(' ')[0].title()
path =os.getcwd() + '/' + name
gitignore = path + '/.gitignore'
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
	if not os.path.exists(path):
		os.makedirs(name)
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
	os.chdir(path)
	download_gitignore()
	nolang = lambda: msg('red', 'Your language isn\'t in my database')
	languages[lang](path, name) if lang in languages.keys() else nolang()

create_project()