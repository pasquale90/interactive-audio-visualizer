#!bin/bash
make clean
make

# source ~/miniconda3/etc/profile.d/conda.sh
# conda activate melEnv

################################################################ ARGUMENTS ################################################################
audio_device=K6
sample_rate=44100 #16000 #
buffer_size=512 
###########################################################################################################################################
startJack_command="jackd --realtime --verbose -t 10000 -d alsa"
startJack_command="${startJack_command} -d hw:${audio_device}"
startJack_command="${startJack_command} -r ${sample_rate}"
startJack_command="${startJack_command} -p ${buffer_size}"
echo -e "\nrunning startJack_command >> \n${startJack_command}"

# # RUN SUDO INSTRUCTIONS - DEACTIVATED
# export HISTIGNORE='*sudo -S*'
# sudopass=${1}
# if [ -z "$1" ]; then
#     >&2 echo "No password provided"
#     exit 1
# fi

# start_jackd
# start_server="jackd --realtime --verbose -t 10000 -d alsa -d hw:K6 -r 16000 -p 512"
# echo ${sudopass} | sudo -S -k nohup ${startJack_command} &> logs/audioServer.log & 
nohup ${startJack_command} &> logs/audioServer.log &

sleep 1

bash welcome

# echo ${sudopass} | sudo -S -k ./test ${sample_rate} ${buffer_size} &> logs/audio.log &
./test ${sample_rate} ${buffer_size} &> logs/audio.log &

echo -e "Application is running... Run $ \033[32;1;4mbash\033[0m \033[32;1;4mstop.sh\033[0m to stop it. "