#!/bin/bash
# chmod +x run_release.sh          

clear && ./build-release/bin/gp_time_sender -t=2 -d=my_test_db -w=https://echo.free.beeceptor.com/ -p=80 -v -c