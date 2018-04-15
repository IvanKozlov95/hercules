./manger &

sleep 3

kill -s KILL $(pgrep podargos lampon xanthos deinos)
kill -s KILL $(pgrep yes)
kill -s KILL $(top -l 1 | grep manger | awk '{print $1}')

rm -rf crap*