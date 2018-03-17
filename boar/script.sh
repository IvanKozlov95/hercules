#/bin/bash
#sudo launchctl load -w /System/Library/LaunchDaemons/com.apple.atrun.plist
echo "Make sure atrun daemon is running. (man atrun)"
echo "Time and date are $(date) and I've been run." >> /tmp/psss.txt | at 0842 Dec 21 2018