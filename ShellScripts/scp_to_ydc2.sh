#!/usr/bin/env bash
cd `dirname $0`
cd ../
folder_path=`pwd`
folder_name=`basename $folder_path`
log_file_name=`date "+%Y%m%d_%H%M%S_log.csv"`

# echo $folder_path
# echo $folder_name
# echo $log_file_name

cd ../
scp.exe -r $folder_name genetec@etrobo-ydc2.local:work/RasPike/sdk/workspace/