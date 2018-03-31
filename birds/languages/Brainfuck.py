import os
import sys

class	Brainfuck:
	def __init__(self, path, name):
		file = open(path + '/tryme.sh','w+')
		file.write("echo \"I'm sorry for you pal...\"")
		file.close()