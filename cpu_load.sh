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

prev_total=0
prev_idle=0

while true; do

   sleep 2

   cpu_data=(`sed -n 's/^cpu\s//p' /proc/stat`)
   idle_time=${cpu_data[3]}
   io_wait=${cpu_data[4]}

   idle_time=$((idle_time+io_wait))

   total_ticks=0

   for value in "${cpu_data[@]}"; do
      total_ticks=$((total_ticks+value))
   done

   diff_idle=$((idle_time-prev_idle))
   diff_total=$((total_ticks-prev_total+1))
   diff_usage=$((diff_total-diff_idle))
   diff_usage=$((diff_usage*100))
   diff_usage=$((diff_usage/diff_total))

   # update at the same location
   echo -en "\r cpu : $diff_usage %  \b\b"

   prev_total=$total_ticks
   prev_idle=$idle_time

done
