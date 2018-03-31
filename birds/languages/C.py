import os
import sys
from util import *

makefile = 'Makefile'

class	C:
	def __init__(self, dir, name):
		self.dir = dir
		self.name = name
		self.create_makefile()
		self.create_folder_structure()

	def create_makefile(self):
		NAME = self.name
		file = open(self.dir + '/Makefile','w+')
		make_lft = '\tmake -C libft/\n'
		flag_lft = '-L libft -lf'
		text = '# MAKEFILE : ' + NAME + '\n\n' + 'NAME = ' + NAME + '\n\n' 
		text += 'SRC += main.c' + '\n\n' + 'OBJ = $(SRC:.c=.o)\n'
		text += '$(NAME): $(OBJ)\n' + make_lft + "\t$(CC) $(OBJ) -o $(NAME) " + flag_lft + '\n\n'
		file.write(text)
		file.close()

	def create_folder_structure(self):
		pass
	
	def clone_libft(self):
		pass