 #! /bin/bash
#
#simpleUnitTest.sh
#
########################################
# Update July 2012
# Update August 2016
# Adapted November 2017
# No restriction on usage nor dissemination
########################################
#
# Help : see readme file
# Bugs and questions : castagne@imag.fr
########################################



# Usage string
USAGE="Usage: `basename $0` -e <executablefile> [-b] [-p] [-d] [-q] [-v] [-o <summaryfile>] [-B] [-w] [-c] [-s <skip_file>] <args_testfiles>"

#Prints usage
printUsage()
{
	echo $USAGE
}

#Prints help
printHelp()
{
	echo
	echo $USAGE
	echo
	echo "`basename $0` runs the <executablefile> specified in argument onto each of the test files specified in argument and checks the results"
	echo 
        echo "The employed <executablefile> is assumed to print its results onto <testfile>.l."
        echo " "
        echo "Each <testfile> in <args_testfiles> should contain, in comment:"
        echo "   # TEST_RETURN_CODE=X where X is in {PASS,FAIL,DO_NOT_CHECK}"
        echo "and optionnaly :"
        echo "   # TEST_COMMENT=string that provides information about the test"
        echo " "
	echo "Each test <testfile> in <args_testfiles> should be accompanied, in the same directory, with"
        echo "  a file named <testfile>.res containing the expected standard output of the tested program for this <testfile>;"
	echo " even if the executable is not supposed to output anything."
	echo "  "
	echo " The <executablefile> will be run on each <testfile> and :"
        echo "	1/ compare the return code value of the program with the expected return code value (PASS, ie 0, or FAIL, ie non 0)"
        echo "	2/ generate a file <testfile>.l containing the output of the program "
        echo "	3/ compare the expected output (<testfile>.res) with the output of the tested program (<testfile>.l)"
        echo "	4/ display results and generate a report"
	echo ""
	echo " Arguments :"
	echo "	simpleUnitTest [-h] -e <executablefile> [options] <args_testfiles>"
	echo " Where :"
	echo "	-e <executablefile> : name of the executable to test"
	echo " And 	"
	echo "	args_testfiles	: list of test files on which to run the executable"
        echo " Options are :"
	echo "	-h				: display help message"
	echo "	-b				: batch mode : no user interaction"
	echo "	-p				: pause mode : adds some delay in between two tests"
	echo "	-d				: display debugs of tested program (display stderr of tested program)"
	echo "	-q				: quiet mode : much less output while testing"
        echo "	-v 				: verbose outputs (adds the test file and the out file)"
        echo "	-c				: cleans the testing files before running :"
	echo "							removes <summaryfile>"
	echo "							removes any test output eg testfile.l"
	echo "	-o <summaryFile>		: if used, writes the results into summaryfile"
        echo "	-B				: if used, -B is passed to diff when comparing the results. See man diff"
        echo "	-w				: if used, -w is passed to diff when comparing the results. See man diff"
        echo "	-s <file>			: skip mode. Allows to interupt a test suite, then restart it later."
	echo "						Stores the ran test in <file> ; skip a test if already present in <file>."
	echo
}

#####################################


#Global variables : constants
TRUE=0
FALSE=1

#Global variables : required parameters
TEST_FILES=""
EXE_PATH=""

#Global variables : options
OUTPUT_FILE=""
OPT_BATCH_MODE=$FALSE
OPT_CLEAN_ENVIRONMENT_BEFORE_RUN=$FALSE
DIFF_OPTIONS=""
OPT_QUIET_MODE=$FALSE
OPT_DISPLAY_PRG_DEBUG=$FALSE
OPT_BATCH_PAUSE=$FALSE
OPT_VERBOSE_OUTPUT=$FALSE

#Global variables : result summary
TEST_PASSED_COUNT=0
TEST_SKIPPED_COUNT=0
TEST_FAILED_COUNT=0

TEST_PASSED=""
TEST_SKIPPED=""
TEST_FAILED=""

#Variable to store failed test with some details on failure
TEST_FAILED_SYS_ERR_CODE=""
TEST_FAILED_ERR_CODE=""
TEST_FAILED_OUTPUT_DIFFERS=""

#Global variables : output colors
BASE_COLOR="\\033[1;35m" #green
HIGHLIGHT_COLOR="\\033[1;31m" #red

#ajout du rep courant au PATH
PATH=$PATH:.

#Progress mode
SKIP_FILE=""


#############################
#Parse command line .
#############################
parseCommandLine() {
    #option
	while getopts e:o:hbqcdxwvps: OPT
	do
		case "$OPT" in
		h)
			printHelp
                        exit 0
			;;
		b)
			OPT_BATCH_MODE=$TRUE
			;;
		p)
			OPT_BATCH_PAUSE=$TRUE
			;;
		q)
			OPT_QUIET_MODE=$TRUE
                        ;;
                v)
                        OPT_VERBOSE_OUTPUT=$TRUE
                        ;;
		d)
			OPT_DISPLAY_PRG_DEBUG=$TRUE
			;;
		c)
			OPT_CLEAN_ENVIRONMENT_BEFORE_RUN=$TRUE
			;;
		e)
			EXE_PATH="$OPTARG"
			;;
		o)
			OUTPUT_FILE="$OPTARG"
                        ;;
                B)
                        DIFF_OPTIONS="$DIFF_OPTIONS -B"
			;;
		w)
			DIFF_OPTIONS="$DIFF_OPTIONS -w"
			;;
		s)
			SKIP_FILE="$OPTARG"
			;;
		\?)
			#getopts issues an error message
			echo $USAGE
			exit_err "ERROR invalid arguments"
			;;
		esac
	done

	shift ` expr $OPTIND - 1 `
    #actual arguments : test files
	TEST_FILES=$*
}



#############################
#Check command line options.
#############################
checkOptions() {
	if [ -z $EXE_PATH ]
	then
		printUsage
		exit_err "missing executable file"
	fi
	if [ ! -f $EXE_PATH ]
	then
		exit_err "file $EXE_PATH does not exist"
	fi
    if [ ! -x $EXE_PATH ]
	then
		exit_err "$EXE_PATH is not an executable file"
	fi
	
	if [ -z "$TEST_FILES" ]
	then
		printUsage
		exit_err "missing test file argument"
	fi
}


#############################
#Miscelaneous : help, and tool functions
#############################

#Writes its args both in stdout and at end of OUTPUT_FILE, 
#if OUTPUT_FILE is set


echoOutputBasic () 
{
	echo -E "$@"
	if [ -n "$OUTPUT_FILE" ]
	then
		echo "$@" >> "$OUTPUT_FILE"
	fi 
}


echoOutput () 
{
	echo -e -n "$BASE_COLOR"
	echo -E "$@"
	echo -e -n "\\033[1;0m"
	if [ -n "$OUTPUT_FILE" ]
	then
		echo "$@" >> "$OUTPUT_FILE"
	fi 
}

#Writes its args at end of OUTPUT_FILE, if OUTPUT_FILE is set
# and to stdout if OPT_QUIET_MODE is not set
echoOutputQuiet () 
{
	if [ $OPT_QUIET_MODE == $TRUE ] 
	then
		return
	fi
	echo -e -n "$BASE_COLOR"
	echo -E "$@"
	echo -e -n "\\033[1;0m"
	if [ -n "$OUTPUT_FILE" ]
	then
		echo "$@" >> "$OUTPUT_FILE"
	fi 
}

#Writes its args with HIGHLIGHT_COLOR
#both in stdout and at end of OUTPUT_FILE, 
#if OUTPUT_FILE is set
echoOutputHighlight () 
{
	echo -e -n "$HIGHLIGHT_COLOR"
	echo -E "$@"
	echo -e -n "\\033[1;0m"
	if [ -n "$OUTPUT_FILE" ]
	then
		echo "$@" >> "$OUTPUT_FILE"
	fi 
}

#Prints error and exits with error code
exit_err()
{
	echoOutputHighlight "Error: $@"
	exit 1
}


#Retrieve the PID of the child process, if any.
#Assumes only one child process exists
getChildProcessPid() 
{
	# Seems that we need to call jobs first to get rid of pending 
	# jobs info (eg "process something [killed]"
	jobs > /dev/null
	childpid=`jobs -l | sed -n 's/^\[[0-9]*\] *+  *\([0-9]*\) .*$/\1/p'`
	#childpid=`jobs -l | awk '{print $2}'`
	echo $childpid
}

# Handle SIGINT signals, allowing to deal with e.g. infinite loops in a test :
# If a tests is running, kills it
# Else kills the current script
sigKillHandler() 
{	echo  "***** Signal received - Ctrl+C was pressed"
	childpid=`getChildProcessPid`
#	echo "child $childpid"
	if [ -n "$childpid" ]
	then
		#kill the child
		echoOutputHighlight "killing test with pid $childpid"
		kill -9 $childpid
		return
	fi 
	#exits this shell script
	echoOutputHighlight "Quitting simpleUnitTest"
	exit 130
}


# prepareEnvironment()
# prepares the testing environment by removing
# output files, if -c is passed to the command:
# remove the output file, and any *.l file
prepareEnvironment () {
	#clean environment, if -c was passed as argument
	if [ $OPT_CLEAN_ENVIRONMENT_BEFORE_RUN == $FALSE ]
	then  	
		return
	fi
		
	echoOutputQuiet "Cleaning environement : removing summary (-o) and any .l file"
	
	# remove SUMMARY_FILE
	if [ -n OUTPUT_FILE ]
	then
		rm "$OUTPUT_FILE" 2> /dev/null # no output
	fi
	
	# remove .l file for each test file
	
	for test_file in $TEST_FILES
	do	
		#drop extension from file name
		test_name=${test_file%\.*}
		#build out file name
		out_file_name="$test_name.l"
		rm "$out_file_name"  2> /dev/null # no output
	done
}


#############################
# Functions directly related to each test execution
#############################


# echoTestEnvironnment()
# Outputs the paths to the various files involved into the test about to run
# - the test file itself
# - the .res file containing the expected output for the test
# - the .l file that will be generated
# each test file is passed to the command (3 args)
echoTestEnvironnment()
{
	if [ $# -ne 3 ]
	then
		exit_err "ERROR 54"
	fi
	
	echoOutputQuiet '**************************'
	echoOutputQuiet "TEST ENVIRONMENT :"
	
	if [ ! -r "$1" ]
	then
		echoOutputHighlight "	test file:	$1	Not Readable"
	else		
		echoOutputQuiet		"	test file:	$1	(OK)"
	fi
		
	if [ ! -r "$2" ]
	then
		echoOutputHighlight "	expected result file:	$2	Not Readable"
	else
		echoOutputQuiet		"	expected result file:	$2	(OK)"
	fi		
	
	echoOutputQuiet "	test output file:	$3"
}


echoOutputFile()
{
	if [ $# -ne 2 ]
	then
		exit_err "ERROR 5554 $#"
	fi
	echoOutputQuiet  $1
	echoOutputQuiet '--------------------------'
	data=`cat $2`
	echoOutputBasic "$data"
	echoOutputQuiet '--------------------------'
}

# analyseExitCode()
# Analyses the exit code for the test,
# checking if it is a system return code
# then comparing it to the expected value (as specified in the .info file)
#
# returns 0 if exit code meets expected result
#
# 3 args :
# arg $1 : exe exit code
# arg $2 : info related to expected exit code. 
#          Either PASS or FAIL (as specified in .info file)
#		   or DO_NOT_CHECK if unknown
# arg $3 : test file name, for output
analyseExitCode()
{	if [ $# -ne 3 ]
	then
		exit_err "ERROR 55"
	fi
	
	exe_exit_code=$1
	exe_expected_result=$2
	test_file_name=$3
		
	# detect weird return codes
	# cf. http://tldp.org/LDP/abs/html/exitcodes.html
	if [ $exe_exit_code -ge 126 ]
	then				
		echoOutputHighlight "--> System return code detected : $exe_exit_code"
		case "$exe_exit_code" in
		126)
			echoOutputHighlight "$exe_exit_code : Command cannot execute"
			;;
		127)
			echoOutputHighlight "$exe_exit_code : Command not found"
			;;
		128)
			echoOutputHighlight "$exe_exit_code : Invalid argument to exit"
			;;
		130)
			echoOutputHighlight "$exe_exit_code : Terminated with Ctrl+C"
			;;
		255)
			echoOutputHighlight "$exe_exit_code : Exit status out of range"
			;;
		*)
			sigvalue=`expr $exe_exit_code - 128`
			signame=SIG`kill -l $sigvalue`
			echoOutputHighlight "$exe_exit_code : Fatal error, signal $sigvalue : $signame"
			;;
		esac
		TEST_FAILED_SYS_ERR_CODE="$TEST_FAILED_SYS_ERR_CODE $test_file_name"
		return 1
	fi
	
	
	
	# analyse program return code and compare to expected
	if [ -n "$exe_expected_result" ]
	then
		case "$exe_expected_result" in
		PASS)
			if [ $exe_exit_code == "0" ]
			then
				echoOutputQuiet "--> Return code is correct : $exe_exit_code"
			else
				echoOutputHighlight "--> Return code ERROR detected : $exe_exit_code . Should be 0"
				TEST_FAILED_ERR_CODE="$TEST_FAILED_ERR_CODE $test_file_name"
				return 2
			fi 
			;;
		FAIL)
			if [ $exe_exit_code -ne "0" ]
			then
				echoOutputQuiet "--> Return code is correct : $exe_exit_code"
			else				
				echoOutputHighlight "--> Return code ERROR detected : $exe_exit_code . Should be non-0"
				TEST_FAILED_ERR_CODE="$TEST_FAILED_ERR_CODE $test_file_name"
				return 3
			fi 
			;;
		DO_NOT_CHECK)
			echoOutputHighlight "--> Return code unchecked."
			;;
		*)
			echoOutputHighlight "Expected return code $exe_expected_result (in the test file) is not a valid token... Cannot check return code."
			echoOutput          "Expected return code should be PASS or FAIL or DO_NOT_CHECK."
			echoOutput          "Cannot analyse return code"
			TEST_FAILED_ERR_CODE="$TEST_FAILED_ERR_CODE $test_file_name"
			return 4
			;;
		esac
	fi 
	#ret code OK
	return 0
}

# executeTest()
# Execute the test on file $1
# - checks that the test environment exists and is valid
#	There should be one script test file
#               starting with a comment indicating TEST_RETURN_CODE and TEST_COMMENT
#	There should be one .res file (with expected output)
#	Test is "SKIPPED" if there is an error.
#
# - launch the test in background
# - checks the test exit code
# - checks the test output
#
# Returns 0 if test passes
# Returns 1 if test fails
# Returns 2 if test skipped
#
# 1 args :
# arg $1 : the test file path
executeTest()
{
	if [ $# -ne 1 ]
	then
		exit_err "ERROR 56"
	fi
	
	test_file_name=$1
	echoOutputHighlight "TESTING         $test_file_name"
	
		
	#drop extension from file name
	test_name=${test_file_name%\.*}
	#build other file names
	res_file_name="$test_name.res"
	out_file_name="$test_name.l"
	
	#retrieving the test info from the test script
	unset TEST_RETURN_CODE
	unset TEST_COMMENT
	
	
	#the test file should contains the two variables 
	# TEST_RETURN_CODE and TEST_COMMENT 
	#	The code below extract them from the script
	
	TEST_RETURN_CODE=$(sed -ne '/^#.*TEST_RETURN_CODE\s*=\s*\([A-Z]*\).*/s/^#.*TEST_RETURN_CODE\s*=\s*\([A-Z]*\).*/\1/p' $test_file_name)

        # extract the first (if multiple TEST_RETURN_CODE are provided)
        TEST_RETURN_CODE=`echo $TEST_RETURN_CODE | head -n1 | awk '{print $1;}'`

	TEST_COMMENT=$(sed -ne '/^;.*TEST_COMMENT\s*=\s*\(.*\).*/s/^;.*TEST_COMMENT\s*=\s*\(.*\).*/\1/p' $test_file_name)
        
	if [ -z "$TEST_RETURN_CODE" ]
	then
		TEST_RETURN_CODE=UNKNOWN
	fi
	
	if [ -n "$TEST_COMMENT" ]
	then
		echoOutput "	$TEST_COMMENT"
	fi
	
	echoTestEnvironnment $test_file_name $res_file_name $out_file_name



	echoOutputQuiet '**************************'
	
	if [ $OPT_BATCH_MODE == $FALSE -a $OPT_QUIET_MODE == $FALSE ]
	then  	
		echo "press Enter to start the test..." 	
		read
	fi
	if [ $OPT_BATCH_PAUSE == $TRUE ]
	then
		sleep 1	
	fi

	# testing if test environment is correct...
	if [ ! -r "$test_file_name" ]
	then
		echoOutputHighlight "SKIPPING: test file $test_file_name is not a readable file"
		TEST_SKIPPED="$TEST_SKIPPED $test_file_name"
		TEST_SKIPPED_COUNT=`expr $TEST_SKIPPED_COUNT + 1 `
		return 2
	fi
	
	if [ ! -r "$res_file_name" ]
	then
		echoOutputHighlight "SKIPPING: result file $res_file_name is not a readable file"
		TEST_SKIPPED="$TEST_SKIPPED $test_file_name"
		TEST_SKIPPED_COUNT=`expr $TEST_SKIPPED_COUNT + 1 `
		return 2
	fi	
		
	#we assume the test is OK
	TEST_OK=$TRUE
	echoOutputQuiet
	echoOutputQuiet '**************************'
	echoOutputQuiet "Starting test..."
	
	#runing the test in background
	if [ $OPT_DISPLAY_PRG_DEBUG == $TRUE ] 
	then
		$EXE_PATH $test_file_name > /dev/null  &
	else
		$EXE_PATH $test_file_name > /dev/null  &
	fi
	
	#$! contient le PID (process id) du dernier process lance en tache de fond
	# => ici du programme teste lance avec "&" ci dessus
	TEST_BASH_PID=$!

		
	#waiting for the test to finish
	# which contains the pid of the last started process

	# getChildProcessPid bug => 
	# si le subprocess est deja termine, getChildProcessPid retourne 0
	# wait `getChildProcessPid` 
	wait $TEST_BASH_PID

	# wait returns the return code of the process...
	exe_exit_code=$?
	
	
	# outputs data set
	if [ $OPT_VERBOSE_OUTPUT == $TRUE ]
	then
		echoOutputFile "--> Test data set" $test_file_name 
	fi
	
	# outputs test results
	if [ $OPT_VERBOSE_OUTPUT == $TRUE ]
	then
		echoOutputFile "--> Test out file" $out_file_name 
	fi

	# analyse return codes
	echoOutputQuiet '**************************'
	analyseExitCode $exe_exit_code $TEST_RETURN_CODE $test_file_name
	ret_code_ok=$?
	if [ $ret_code_ok -ne 0 ]
	then
		TEST_OK=$FALSE
	fi




	# analyse output, by using diff	
	echoOutputQuiet '**************************'
	diff $DIFF_OPTIONS $out_file_name $res_file_name > /dev/null
	if [ $? -ne 0 ] && [ "$TEST_RETURN_CODE" = "PASS" ]
	then
		TEST_OK=$FALSE
		TEST_FAILED_OUTPUT_DIFFERS="$TEST_FAILED_OUTPUT_DIFFERS $test_file_name"

		echoOutputHighlight "--> Test output is not correct. Compare $res_file_name and $out_file_name for details."
		echoOutputQuiet "Here is the diff:"
		echo "`diff --context=2 $DIFF_OPTIONS $out_file_name $res_file_name`"
		diff --context=2 $DIFF_OPTIONS $out_file_name $res_file_name | echoOutputQuiet
	fi
	
	# finalization
	echoOutputQuiet '**************************'
	if [ $TEST_OK == $TRUE ]
	then
		echoOutputHighlight "TEST SUCCESSES  :  $test_file_name"	
		TEST_PASSED="$TEST_PASSED $test_file_name"
		TEST_PASSED_COUNT=`expr $TEST_PASSED_COUNT + 1 `
		return 0
	fi
		
	echoOutputHighlight "===> TEST GOES WRONG : $test_file_name "	
	TEST_FAILED="$TEST_FAILED $test_file_name"
	TEST_FAILED_COUNT=`expr $TEST_FAILED_COUNT + 1 `
	return 1
}


menuPostTest () 
{
	if [ $# -ne 2 ]
	then
		exit_err "ERROR 56"
	fi
	
	error_code=$1
	test_file_name=$2
	
	if [ "$error_code" == "0" ] 
	then
		echo "press enter to go to next test"
		read
		return 
	fi
			
	#drop extension from file name
	test_name=${test_file_name%\.*}
	#build other file names
	res_file_name="$test_name.res"
	out_file_name="$test_name.l"
	
	while :
	do
		echo '******************************'
		echo "Choose option:"
		echo "   <enter>: go to next test"
		echo "   1      : display test file $test_file_name"
		echo "   2      : display res  file $res_file_name"
		echo "   3      : display out  file $out_file_name"
		echo "   4      : display diff between out and res files"
		printf "your choice: "
		
		read menucode 
		echo '******************************'
		case "$menucode" in
		1)
			cat "$test_file_name"
			;;
		2)
			cat "$res_file_name"
			;;
		3)
			cat "$out_file_name"
			;;
		4)
			diff --context=2 $DIFF_OPTIONS "$out_file_name" "$res_file_name"
			;;
		"")
			return
		esac
	done
}



#############################
# Here we go ! Main program
#############################

parseCommandLine $@
checkOptions


echoOutputQuiet '**************************'
echoOutputQuiet "Welcome to $0"
echoOutputQuiet "Testing executable $EXE_PATH on test files :"
echoOutputQuiet "     $TEST_FILES"
echoOutputQuiet "To quit a running test, hit CTRL+C"
echoOutputQuiet '**************************'

prepareEnvironment
trap 'sigKillHandler' INT


# while running the test suite, SIGINT (ctrl+C) signal is traped,
# and used to interupt the test
# instead of interupting this shell interpreter


if [ $OPT_BATCH_MODE == $FALSE -a $OPT_QUIET_MODE == $FALSE ]
then  	
		echo "press Enter to start..." 	
		read
fi
if [ $OPT_BATCH_PAUSE == $TRUE ]
then
	sleep 1	
fi

for test_file in $TEST_FILES
do	
	echoOutputQuiet ""
	echoOutputQuiet ""
	echoOutputQuiet ""
	echoOutputQuiet ""
	echoOutputQuiet "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echoOutput      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	
	if [ -n "$SKIP_FILE" ]
	then
		grep "$test_file" "$SKIP_FILE" > /dev/null
		if [ $? == "0" ]
		then
			echoOutputHighlight "Skip test $test_file : found in skip file $SKIP_FILE"
			TEST_SKIPPED="$TEST_SKIPPED $test_file_name"
			TEST_SKIPPED_COUNT=`expr $TEST_SKIPPED_COUNT + 1 `

			if [ $OPT_BATCH_PAUSE == $TRUE ]
			then
				sleep 1	
			fi
			continue
		fi
	fi
	executeTest $test_file
	retValue=$?
	
	if [ $OPT_BATCH_MODE == $FALSE ]
	then
		menuPostTest $retValue $test_file
	fi
	
	if [ $OPT_BATCH_PAUSE == $TRUE ]
	then
		sleep 1	
	fi
	
	if [ -n "$SKIP_FILE" ]
	then
		echoOutputQuiet "Storing test $test_file in $SKIP_FILE"
		echo "$test_file" >> $SKIP_FILE
	fi
	echoOutputQuiet "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echoOutputQuiet "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
done

TEST_COUNT=`expr $TEST_PASSED_COUNT + $TEST_FAILED_COUNT + $TEST_SKIPPED_COUNT`


echoOutput
echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echoOutput "Summary :"

echoOutput "SKIPPED TESTS:"
echoOutput "	$TEST_SKIPPED"
echoOutput "SUCCESSFULL  TESTS:"
echoOutput "	$TEST_PASSED"

echoOutput "INCORRECT    TESTS:"
echoOutput "	$TEST_FAILED"
if [ -n "$TEST_FAILED" ]
then
	echo "Among which :"
	echo "Tests with system error code (eg : segmentation faults, aborts...):"
	echo "	$TEST_FAILED_SYS_ERR_CODE"
	echo "Tests with incorrect return code:"
	echo "	$TEST_FAILED_ERR_CODE"
	echo "Tests with incorrect output:"
	echo "	$TEST_FAILED_OUTPUT_DIFFERS"
fi


echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echoOutput "TEST SUCCESSFUL  : $TEST_PASSED_COUNT    out of $TEST_COUNT "
echoOutput "TEST WRONG       : $TEST_FAILED_COUNT    out of $TEST_COUNT "
echoOutput "TEST SKIPPED     : $TEST_SKIPPED_COUNT    out of $TEST_COUNT "
echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echoOutput "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

if [ "$TEST_PASSED_COUNT" == "$TEST_COUNT" ]
then
	exit 0
fi
	
exit 1
