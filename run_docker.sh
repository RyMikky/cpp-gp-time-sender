#!/bin/bash
# chmod +x run_docker.sh          

clear && sudo docker run --rm gp_time_sender -t=1 -d=my_sqlite3_database -w=https://echo.free.beeceptor.com/ -p=80 -c