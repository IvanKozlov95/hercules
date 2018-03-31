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

def download_gitignore():
	link = 'https://raw.githubusercontent.com/github/gitignore/master/' + lang + '.gitignore'
	print(link)
	os.system('curl ' + link + ' --output .gitignore')
	file = open(gitignore)
	if '404' in file.read():
		os.remove(gitignore)
		msg('red', 'Couldn\'t find gitignore for your language, sorry.')

def mkdir():
	if not os.path.exists(path):
		os.makedirs(name)
		msg('green', 'Created directory ' + path + '\n')
	else:
		msg('yellow', 'Directory ' + path + ' already exists.')
		yn = input('Continue? (y/n)?\n')
		while (yn != 'y'):
			if yn in 'nN':
				msg('red', 'Aborting...')
				return
			else:
				msg('white', 'Please answer yY or nN.')
			yn = input('Continue? (y/n)?\n')

def create_project():
	mkdir()
	os.chdir(path)
	download_gitignore()
	{
		'C': C(path, name)
	}[lang]
	pass

create_project()