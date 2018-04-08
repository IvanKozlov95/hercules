.name "zork"
.comment "I'M ALIIIIVE"

l2:		sti r1, %:live, %1
		and r1, %0, r1

live:	live %1
		zjmp %:live

//
fork %:grow

ld %1, r3
ld %0, r4

l2:		sti r1, %:live, %1
		and r2, %0, r2

grow:	sti r1, %1, %0
		ld %:grow, r3
		sti r3, %10, %0
		fork %:grow

live:	live %1
		zjmp %:live
