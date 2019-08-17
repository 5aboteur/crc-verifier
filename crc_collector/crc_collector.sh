#!/bin/bash

set -e

collected_cnt=0
crc_db="${PWD}/crc_db"

traverse() {
	for file in $1/*
	do
		# Collect only executables
		if [[ -f ${file} && -x ${file} && ! -L ${file} ]]; then
			# Calculate crc value with this portable solution, taken from here
			# https://stackoverflow.com/questions/44804668/how-to-calculate-crc32-checksum-from-a-string-on-linux-bash
			crc=$(gzip -c < "${file}" | tail -c8 | od -t x4 -N 4 -A n)

			# Insert pair {full_file_name} <=> {crc}
			echo "${file}${crc}" >> "${crc_db}"

			# Increase collected files counter
			collected_cnt=$((collected_cnt+1))
		elif [[ -d ${file} ]]; then
			traverse "${file}"
		fi
	done
}

echo "==> Run crc collector tool"

# Clear database
true > "${crc_db}"

cd "$1"
traverse "."

echo "==> Collected files: ${collected_cnt}"
