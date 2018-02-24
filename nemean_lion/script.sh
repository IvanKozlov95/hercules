apt-get install openssh-server
cat /etc/ssh/sshd_config | sed 's/.*Port.*/Port 420/' > ./asd
mv -f ./asd /etc/ssh/sshd_config
systemctl restart sshd
