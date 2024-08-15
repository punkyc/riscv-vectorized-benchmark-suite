#!/bin/bash

# Array of directories
directories=("_axpy" "_blackscholes" "_canneal" "_particlefilter" "_pathfinder" "_streamcluster" "_swaptions")

# Function to execute files in a directory
execute_files() {
    dir=$1
    for file in "$dir"/*; do
        if [[ -x "$file" && -f "$file" ]]; then
            case "$file" in
                *_axpy*)
                    echo "Running _axpy: $file with input parameters: 256"
                    #~/spike --isa=rv64gcv ~/pk $file 256
                     "$file" 256
                    ;;
                *_blackscholes*)
                    echo "Running _blackscholes: $file with input parameters: 1 ./input/in_64K.input ./prices.txt"
                    #~/spike --isa=rv64gcv ~/pk $file 1 ./_blackscholes/input/in_64K.input ./_blackscholes/prices.txt
                    "$file" 1 ./_blackscholes/input/in_64K.input ./_blackscholes/prices.txt
                    ;;
                *_canneal*)
                    echo "Running _canneal: $file with input parameters: 1 100 300 input/100.nets 8"
                    #~/spike --isa=rv64gcv ~/pk $file 1 100 300 input/100.nets 8
                    "$file" 1 100 300 input/100.nets 8
                    ;;
                *_particlefilter*)
                    echo "Running _particlefilter: $file with input parameters: -x 128 -y 128 -z 2 -np 256"
                    #~/spike --isa=rv64gcv ~/pk $file -x 128 -y 128 -z 2 -np 256
                    "$file" -x 128 -y 128 -z 2 -np 256
                    ;;
                *_pathfinder*)
                    echo "Running _pathfinder: $file with input parameters : 32 32 out"
                    #~/spike --isa=rv64gcv ~/pk $file 32 32 out
                    "$file" 32 32 out
                    ;;
                *_streamcluster*)
                    echo "Running _streamcluster: $file with input parameters :3 10 128 128 128 10 none output.txt 1"
                    #~/spike --isa=rv64gcv ~/pk $file 3 10 128 128 128 10 none output.txt 1
                    "$file" 3 10 128 128 128 10 none output.txt 1
                    ;;
                *_swaptions*)
                    echo "Running _swaptions: $file with input parameters: -ns 8 -sm 512 -nt 1"
                    #~/spike --isa=rv64gcv ~/pk $file -ns 8 -sm 512 -nt 1
                    "$file" -ns 8 -sm 512 -nt 1
                    ;;
                *)
                    echo "No known input parameters for $file"
                    ;;
            esac
        fi
    done
}

# Iterate over each directory and execute files
for dir in "${directories[@]}"; do
    if [ -d "$dir" ]; then
        execute_files "$dir"
    else
        echo "Directory $dir does not exist."
    fi
done
