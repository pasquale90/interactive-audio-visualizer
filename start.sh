#!bin/bash
make clean
make

export HISTIGNORE='*sudo -S*'
sudopass=${1}
if [ -z "$1" ]; then
    >&2 echo "No password provided"
    exit 1
fi

# start_jackd
start_server="jackd --realtime --verbose -t 10000 -d alsa -d hw:K6 -r 16000 -p 512"
echo ${sudopass} | sudo -S -k nohup ${start_server} &> logs/audioServer.log &

echo ${sudopass} | sudo -S -k ./test &> logs/audio.log &

echo "Application is running."
echo -e "Run \n\t$ bash stop.sh \nto stop it."