#!/bin/bash
for i in build/bin/*;
  do name=`echo "$i" | cut -d'.' -f1`
  echo "$name"
  $name
  ppm_out=$(echo "$i" | cut -d'/' -f3)
  echo "Converting: ${ppm_out}.ppm"
  ffmpeg -i "${ppm_out}.ppm" -update 1 -frames:v 1 "${ppm_out}.png" -y
done

mv *.png /output/