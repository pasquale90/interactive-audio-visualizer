#!bin/bash
rm -r build demo
mkdir build
cd build
cmake ../
cmake --build .
mv demo ../
cd ../

################################################################ ARGUMENTS ################################################################
#---------------------------------------------------------------- system -----------------------------------------------------------------#
audio_device=Generic_1
#----------------------------------------------------------------- audio -----------------------------------------------------------------#
sample_rate=44100 #16000 #
buffer_size=512 
bit_quantization=24
minimum_frequency=300 #55 # A1
maximum_frequency=700 #3520  # A7
#---------------------------------------------------------------- visual -----------------------------------------------------------------#
# display_frames_per_second=25
# display_width=1024
# display_height=${buffer_size}
#---------------------------------------------------------------- camera -----------------------------------------------------------------#
camera_resolution_width=640
camera_resolution_height=480
camera_frames_per_second=30
roi_radius=$( bc -l <<<"0.1*$display_height" )                                      #-> broken - fix it
roi_waiting_seconds=3
tracking_algorithm=0 # 0:CSRT , 1:KFC, 2:BOOSTING
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
if [ ! -z ${10:-${roi_radius}} ]; then args="${args} --roi_radius ${roi_radius}"; fi
if [ ! -z ${11:-${roi_waiting_seconds}} ]; then args="${args} --roi_waiting_seconds ${roi_waiting_seconds}"; fi
if [ ! -z ${12:-${tracking_algorithm}} ]; then args="${args} --tracking_algorithm ${tracking_algorithm}"; fi
if [ ! -z ${13:-${minimum_frequency}} ]; then args="${args} --minimum_frequency ${minimum_frequency}"; fi
if [ ! -z ${14:-${maximum_frequency}} ]; then args="${args} --maximum_frequency ${maximum_frequency}"; fi
# if [ ! -z ${15:-${__next_param__}} ]; then args="${args} --__next_param__ ${__next_param__}"; fi
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

# clear
bash ./scripts/welcome

# echo ${sudopass} | sudo -S -k ./demo ${sample_rate} ${buffer_size} &> logs/audio.log &
./demo ${args} &> logs/audio.log &

echo -e "Application is running... Run $ \033[32;1;4mbash\033[0m \033[32;1;4mstop.sh\033[0m to stop it. (|| for dev -->  press Q on the visual window ) "
echo -e "Run manually with :\n./demo ${args}"
