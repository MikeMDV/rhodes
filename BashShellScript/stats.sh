#! /bin/bash
#
#
#             Author: Michael Marven
#              Class: 
#         Assignment: 
#       Date Created: 01/17/15
# Last Date Modified: 01/18/15
#          File name: stats
#           Overview: A shell script to calculate the median and average values
#                     of integer values in a file. Depending on the option
#                     selected, the script will calculate across rows or down
#                     columns. The script generates temporary files that are
#                     placed in the current working directory.
#                     
#                     Usage: stats {-rows|-cols} [file]
#
#

trap 'rm ./TMP*.$$; exit' SIGHUP SIGINT SIGQUIT SIGTERM

# ----------------------------------------------------------------------------
# Function declarations

# Function to count lines in a file
# Requirements: File name to be the first argument to the function
count_lines_in_file () {
    wc -l < "$1"
}

# Function to count words in a file
# Requirements: File name to be the first argument to the function
count_words_in_file () {
    wc -w < "$1"
}

# Function to print a usage error to standard error and exit the program with
# a status of 1
print_usage_error () {
    printf "Usage: stats {-rows|-cols} [file]\n" 1>&2
    exit 1
}

# Function to print the average and median values for the -rows option
# Requirements: File name to be provided as the first argument; $words_per_line
# and $line_count variables present and set correctly
rows () {
    # Create a variables for temp files for processing
    TMP2=./TMP2.$$
	TMP3=./TMP3.$$
	
	# Calculate which position will be the median
    median=$(((words_per_line + 2) / 2))
    
    printf "Average Median\n"
    
    # Populate temp files with column values and calculate averages
	loop_count=1
	while [ $loop_count -le $line_count ]
	do
		inner_loop_count=1
		total=0
		avg=0
		while [ $inner_loop_count -le $words_per_line ]
		do
			cut -f "$inner_loop_count" $1 > $TMP2
			read_line=1
			while read num
			do
				if [ $read_line -eq $loop_count ]
				then
					total=$(($total + $num))
					echo $num >> "$TMP3"
				fi
				read_line=$((read_line + 1))
			done < "$TMP2"
			inner_loop_count=$((inner_loop_count + 1))
		done
		loop_count=$((loop_count + 1))
		avg=$(((total + (words_per_line / 2)) / words_per_line))
		printf "%d    " "$avg"
		
		# Calculate median value
		med_pos=1
		sort -n "$TMP3" |
		while read median_value
		do
			if [ $med_pos -eq $median ]
			then
				printf "%d\n" "$median_value"
			fi
			med_pos=$((med_pos + 1))
		done
		truncate -s 0 "$TMP3"
	done

    exit 0
}

# Function to print the average and median values for the -cols option
# Requirements: File name to be provided as the first argument; $words_per_line
# and $line_count variables present and set correctly
cols () {
    # Create a temp file for processing
    TMP2=./TMP2.$$
    
    printf "Averages:\n"
    
    # Populate temp file with column values and calculate averages
    loop_count=1
    while [ $loop_count -le $words_per_line ]
    do
        total=0
        avg=0
        cut -f "$loop_count" $1 > $TMP2
        while read num
        do
            total=$(($total + $num))
        done < "$TMP2"
        avg=$(((total + (line_count / 2)) / line_count))
        printf "%d    " "$avg"
        loop_count=$((loop_count + 1))
    done
    printf "\n"
    
    # Calculate which position will be the median
    median=$(((line_count + 2) / 2))
    
    printf "Medians:\n"
    
    # Populate temp file with column values and calculate medians
    loop_count=1
    while [ $loop_count -le $words_per_line ]
    do
        sort -n -k"$loop_count" $1 | cut -f "$loop_count" > $TMP2
        med_count=1
        while read num
        do
            if [ $med_count -eq $median ]
            then
                printf "%d    " "$num"
            fi
            med_count=$((med_count + 1))
        done < "$TMP2"
        loop_count=$((loop_count + 1))
    done
    
    printf "\n"
    
    exit 0
}

# End function declarations
#
# ----------------------------------------------------------------------------
# Begin script

# Test if standard input is present and if so, copy the file to a temp file. If
# it is not present, check if a the correct number of command line options have
# been entered and if so, check if a readable file has been entered as an 
# option. If a readable file has been entered as an option, copy the file to a 
# temp file. If the correct number of command line options have not been 
# entered, or the file is not readable, print an error message.
TMP1=./TMP1.$$
if read -t 0
then
    cat > $TMP1
elif [ $# != 2 ] # Test if the correct number of options entered
then
    print_usage_error
elif [ $# == 2 ] # Test if the stats file is readable and present
then
    if ! [ -r "$2" ] || ! [ -e "$2" ]
    then
        printf "stats: cannot read %s\n" "$2" 1>&2
        exit 1
    else
        cat "$2" > $TMP1 # Copy file to temp file
    fi
fi

# Declare variables to use for counts
line_count=0
word_count=0
words_per_line=0

# Count total words, lines, and calculate words(fields, columns) per line
word_count=$(count_words_in_file "$TMP1")
line_count=$(count_lines_in_file "$TMP1")
words_per_line=$((word_count / line_count))

# Process arguments, exiting with error if the option in $1 is not recognized
while getopts :cr opt
do
    case $opt in
    r)
        rows "$TMP1"
        ;;
    c)
        cols "$TMP1"
        ;;
    '?')
        print_usage_error
        exit 1
        ;;
    esac
done



