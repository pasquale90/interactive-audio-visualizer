#!bin/bash
make clean
make

# source ~/miniconda3/etc/profile.d/conda.sh
# conda activate melEnv

################################################################ ARGUMENTS ################################################################
#---------------------------------------------------------------- system -----------------------------------------------------------------#
audio_device=K6
#----------------------------------------------------------------- audio -----------------------------------------------------------------#
sample_rate=44100 #16000 #
buffer_size=512 
bit_quantization=24
#---------------------------------------------------------------- visual -----------------------------------------------------------------#
# display_frames_per_second=25
# display_width=1024
# display_height=${buffer_size}
#---------------------------------------------------------------- camera -----------------------------------------------------------------#
camera_resolution_width=640
camera_resolution_height=480
camera_frames_per_second=30
roi_offset=$( bc -l <<<"0.1*$display_height" )                                      #-> broken - fix it
roi_waiting_seconds=5
tracking_algorithm=1 # 0:CSRT , 1:KFC, 2:BOOSTING
###########################################################################################################################################

# Preparing arguments for main
args=""
if [ ! -z ${1:-${sample_rate}} ]; then args="${args} --sample_rate ${sample_rate}"; fi
if [ ! -z ${2:-${buffer_size}} ]; then args="${args} --buffer_size ${buffer_size}"; fi
if [ ! -z ${3:-${bit_quantization}} ]; then args="${args} --bit_quantization ${bit_quantization}"; fi
if [ ! -z ${4:-${display_frames_per_second}} ]; then args="${args} --display_frames_per_second ${display_frames_per_second}"; fi
if [ ! -z ${4:-${display_width}} ]; then args="${args} --display_width ${display_width}"; fi
if [ ! -z ${6:-${display_height}} ]; then args="${args} --display_height ${display_height}"; fi
if [ ! -z ${7:-${camera_resolution_width}} ]; then args="${args} --camera_resolution_width ${camera_resolution_width}"; fi
if [ ! -z ${8:-${camera_resolution_height}} ]; then args="${args} --camera_resolution_height ${camera_resolution_height}"; fi
if [ ! -z ${9:-${camera_frames_per_second}} ]; then args="${args} --camera_frames_per_second ${camera_frames_per_second}"; fi
if [ ! -z ${10:-${roi_offset}} ]; then args="${args} --roi_offset ${roi_offset}"; fi
if [ ! -z ${11:-${roi_waiting_seconds}} ]; then args="${args} --roi_waiting_seconds ${roi_waiting_seconds}"; fi
if [ ! -z ${12:-${tracking_algorithm}} ]; then args="${args} --tracking_algorithm ${tracking_algorithm}"; fi
# if [ ! -z ${10:-${__next_param__}} ]; then args="${args} --__next_param__ ${__next_param__}"; fi
echo "args --> ${args}"

# Creating command for JACK SERVER
startJack_command="jackd --realtime --verbose -t 10000 -d alsa"
startJack_command="${startJack_command} -d hw:${audio_device}"
startJack_command="${startJack_command} -r ${sample_rate}"
startJack_command="${startJack_command} -p ${buffer_size}"
echo -e "\nrunning startJack_command >> \n${startJack_command}"

# Applying sudo priviledges
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

clear
bash welcome

# echo ${sudopass} | sudo -S -k ./test ${sample_rate} ${buffer_size} &> logs/audio.log &
./test ${args} &> logs/audio.log &

echo -e "Application is running... Run $ \033[32;1;4mbash\033[0m \033[32;1;4mstop.sh\033[0m to stop it. (|| for dev -->  press Q on the visual window ) "
