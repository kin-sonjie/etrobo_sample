#!/usr/bin/env bash
cd `dirname $0`
cd ../
folder_path=`pwd`
folder_name=`basename $folder_path`
log_file_name=`date "+%Y%m%d_%H%M%S_log.csv"`

# echo $folder_path
# echo $folder_name
# echo $log_file_name

cd ../../../../
make app=$folder_name 2>&1 | grep "error" --color=always


