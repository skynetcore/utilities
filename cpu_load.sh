#!/bin/bash
#
# Copyright 2019 skynetcore
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

PREV_TOTAL=0
PREV_IDLE=0

while true; do

   sleep 2

   CPU_DATA=(`sed -n 's/^cpu\s//p' /proc/stat`)
   IDLE_TIME=${CPU_DATA[3]}

   TOTAL_TICKS=0
   for VALUE in "${CPU_DATA[@]}"; do
      TOTAL_TICKS=$(($TOTAL_TICKS+$VALUE))
   done

   DIFF_IDLE=$(($IDLE_TIME-$PREV_IDLE))
   DIFF_TOTAL=$((TOTAL_TICKS-$PREV_TOTAL))
   DIFF_USAGE=$((100*(($DIFF_TOTAL-$DIFF_IDLE)/($DIFF_TOTAL+1))))
   
   # update at the same location
   echo -en "\r cpu %: $DIFF_USAGE %  \b\b"

   PREV_TOTAL=$TOTAL_TICKS
   PREV_IDLE=$IDLE_TIME
   
done
