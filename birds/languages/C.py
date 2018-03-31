import os
import sys
import operator
from util import *

makefile = 'Makefile'

class	C:
	def __init__(self, _dir, name):
		self.dir = _dir
		self.name = name
		self.clone_libft()
		self.create_makefile()
		self.create_folder_structure()
		self.create_main()

	def create_main(self):
		file = open(self.dir + '/src/main.c', 'w+')
		text = '#include <stdio.h>\n\n'
		text += 'int\t\tmain(void)\n'
		text += '{\n\tprintf("Hello world!\\n");\n}\n'
		file.write(text)
		file.close()

	def create_makefile(self):
		file = open(self.dir + '/Makefile','w+')
		make_lft = '@make -C libft/\n' if self.has_libft(None) else ""
		flag_lft = '-L libft -lft' if self.has_libft(None) else ""
		inc_lft = '-I libft/includes' if self.has_libft(None) else ""
		clean_lft = '@make -C $(LIBFT_DIR) clean\n' if self.has_libft(None) else ""
		fclean_lft = '@make -C $(LIBFT_DIR) fclean\n' if self.has_libft(None) else ""
		text = (
		"""# MAKEFILE : {name}

NAME = {name}
CC = gcc
FLAGS = -Wall -Wextra -Werror
OBJ_DIR = obj/
SRC_DIR = src/
INC_DIR = includes/
SRC_FILES = main.c
OBJ_FILES = $(SRC_FILES:%.c=%.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
INCLUDES = -I $(INC_DIR) {inc_lft}

all: $(NAME)

$(NAME): $(OBJ)
	{make_lft}@$(CC) $(OBJ) -o $(NAME) {flag_lft}

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	{clean_lft}@rm -rf $(OBJ_DIR)
	@echo "[INFO] Objects removed."

fclean: clean
	{fclean_lft}@rm $(NAME)
	@echo "[INFO] Lem-in executable deleted"

re: fclean all

.PHONY: all $(NAME) clean fclean re
"""
		.format(name=self.name, inc_lft=inc_lft, make_lft=make_lft, flag_lft=flag_lft, clean_lft=clean_lft, fclean_lft=fclean_lft))
		file.write(text)
		file.close()

	def create_folder_structure(self):
		if not os.path.exists(self.dir + '/src'):
			os.mkdir(self.dir + '/src')
		if not os.path.exists(self.dir + '/includes'):
			os.mkdir(self.dir + '/includes')
		pass
	
	def clone_libft(self):
		libft = 'https://github.com/IvanKozlov95/libft'
		self.has_libft(False)
		onyes = lambda: (
			os.system('git submodule add ' + libft),
			os.system('git submodule update --init --recursive'),
			self.has_libft(True))
		ask_until_yn('Add libft as a submodule? (y/n)\n', onyes, None)
		pass

	def has_libft(self, value):
		if value is not None:
			self._has_libft = value & True
		return self._has_libft