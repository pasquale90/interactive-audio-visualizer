#!bin/bash
make clean
make

################################################################ ARGUMENTS ################################################################
audio_device=K6
sample_rate=16000
buffer_size=512
startJack_command="jackd --realtime --verbose -t 1000 -d alsa"
startJack_command="${startJack_command} -d hw:${audio_device}"
startJack_command="${startJack_command} -r ${sample_rate}"
startJack_command="${startJack_command} -p ${buffer_size}"
###########################################################################################################################################

export HISTIGNORE='*sudo -S*'
sudopass=${1}
if [ -z "$1" ]; then
    >&2 echo "No password provided"
    exit 1
fi

echo -e "\nrunning startJack_command >> \n${startJack_command}"
# start_jackd
# start_server="jackd --realtime --verbose -t 10000 -d alsa -d hw:K6 -r 16000 -p 512"
echo ${sudopass} | sudo -S -k nohup ${startJack_command} &> logs/audioServer.log & 

sleep 2

echo ${sudopass} | sudo -S -k ./test ${sample_rate} ${buffer_size} &> logs/audio.log &

echo "Application is running."
echo -e "Run \n\t$ bash stop.sh \nto stop it."