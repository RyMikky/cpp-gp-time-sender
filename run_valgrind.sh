#!/bin/bash
# chmod +x run_valgrind.sh          

clear && valgrind --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes ./build-release/bin/gp_time_sender -u