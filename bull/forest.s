.name "forest"
.comment "1forest1"

l2:
	sti r1, %:live, %1
	sti r1, %:live2, %1
	and r1, %0, r2
	fork %:live2

live:
	live %1
	zjmp %:live

live2:
	live %1
