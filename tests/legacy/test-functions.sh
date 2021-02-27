#!/usr/bin/env bash
#
# unit test functions
#

##
# Realpath polyfill for systems where it doesn't exist
#
# @param $1 Path to examine
#

if [ ! -x "$(command -v realpath)" ] ; then
	function realpath()
	{
		readlink -f $1
	}
fi

##
# Executes a single test against a detoxrc, tables, and sequence.
#
# @param $1 Detox Executable
# @param $2 Input Filename
# @param $3 Output Filename
# @param $4 Path to Config File
# @param $5 Sequence to Run (leave blank to use default)
#
# @return int 0 for success, 1 for failure
#
function test_detoxrc ()
{
	if [ -z "$1" -o -z "$2" -o -z "$3" -o -z "$4" ] ; then
		echo missing parameters
		return 1
	fi

	local DETOX=$1
	local INPUT=$2
	local OUTPUT=$3
	local DETOXRC=$4
	local SEQUENCE=$5

	if [ ! -x "$DETOX" ] ; then
		echo $DETOX is not executable
		return 1
	fi

	if [ ! -f "$DETOXRC" ] ; then
		echo $DETOXRC is not a file
		return 1
	fi

	if [ -n "$USE_VALGRIND" ] ; then
		VALGRIND=$(command -v valgrind || true)

		if [ -z "$VALGRIND" ] ; then
			echo "USE_VALGRIND specified, but valgrind is not installed"
			exit 1
		else
			DETOX="$VALGRIND --quiet --error-exitcode=1 --track-origins=yes $DETOX"
		fi
	fi

	local BASE=/tmp/detoxtest/
	if [ ! -d $BASE ] ; then
		mkdir $BASE
	fi
	local WORK=$(realpath $(mktemp -d $BASE/test-XXXXXX))

	local RC=$WORK/detoxrc

	cp $DETOXRC $RC

	cd $WORK

	local ACTION="touch"
	local OPERATOR="-f"
	local CMDINPUT="$INPUT"
	if [ "${INPUT: -1}" = "/" ] ; then
		ACTION="mkdir"
		OPERATOR="-d"
		CMDINPUT="${INPUT:0: -1}"
	fi

	$ACTION -- "$CMDINPUT"
	if [ -n "$SEQUENCE" ] ; then
		$DETOX -s $SEQUENCE -f $RC -- "$CMDINPUT"
	else
		$DETOX -f $RC -- "$CMDINPUT"
	fi
	if [ ! $OPERATOR "$OUTPUT" ] ; then
		echo renaming "$INPUT" to "$OUTPUT" failed
		return 1
	fi

	return 0
}

##
# Executes a single test against a single function using a single table.
#
# @param $1 Detox Executable
# @param $2 Input Filename
# @param $3 Output Filename
# @param $4 Method (utf_8, safe, etc)
# @param $5 Table (path to the table to use)
#
# @return int 0 for success, 1 for failure
#
function test_single_table ()
{
	if [ -z "$1" -o -z "$2" -o -z "$3" -o -z "$4" -o -z "$5" ] ; then
		echo missing parameters
		return 1
	fi

	local DETOX=$1
	local INPUT=$2
	local OUTPUT=$3
	local METHOD=$4
	local TABLE=$5

	if [ ! -x "$DETOX" ] ; then
		echo $DETOX is not executable
		return 1
	fi

	if [ ! -f "$TABLE" ] ; then
		echo $TABLE is not a file
		return 1
	fi

	if [ -n "$USE_VALGRIND" ] ; then
		VALGRIND=$(command -v valgrind || true)

		if [ -z "$VALGRIND" ] ; then
			echo "USE_VALGRIND specified, but valgrind is not installed"
			exit 1
		else
			DETOX="$VALGRIND --quiet --error-exitcode=1 --track-origins=yes $DETOX"
		fi
	fi

	local BASE=/tmp/detoxtest/
	if [ ! -d $BASE ] ; then
		mkdir $BASE
	fi
	local WORK=$(realpath $(mktemp -d $BASE/test-XXXXXX))

	local TMPTABLE=$WORK/test.tbl
	cp $TABLE $TMPTABLE

	local SEQUENCE=unittest
	local RC=$WORK/detoxrc

	cat <<- DONE > $RC
	sequence $SEQUENCE {
		$METHOD {
			filename "$TMPTABLE";
		};
	};
	DONE

	cd $WORK

	local ACTION="touch"
	local OPERATOR="-f"
	local CMDINPUT="$INPUT"
	if [ "${INPUT: -1}" = "/" ] ; then
		ACTION="mkdir"
		OPERATOR="-d"
		CMDINPUT="${INPUT:0: -1}"
	fi

	$ACTION -- "$CMDINPUT"
	$DETOX -s $SEQUENCE -f $RC -- "$CMDINPUT"
	if [ ! $OPERATOR "$OUTPUT" ] ; then
		echo renaming "$INPUT" to "$OUTPUT" failed
		return 1
	fi

	return 0
}

##
# Executes a single test against one or more functions in a sequence, using the
# stock tables when applicable, from source control.
#
# @param $1 Detox Executable
# @param $2 Input Filename
# @param $3 Output Filename
# @param $4 Path to Stock Tables
# @param $5 First method (utf_8, safe, etc)
# @param ... Additional methods
#
# @return int 0 for success, 1 for failure
#
function test_sequence ()
{
	if [ -z "$1" -o -z "$2" -o -z "$3" -o -z "$4" -o -z "$5" ] ; then
		echo missing parameters
		return 1
	fi

	local DETOX=$1
	local INPUT=$2
	local OUTPUT=$3
	local TABLEPATH=$4
	shift 4

	if [ ! -x "$DETOX" ] ; then
		echo $DETOX is not executable
		return 1
	fi

	if [ ! -d "$TABLEPATH" ] ; then
		echo $TABLEPATH is not a dir
		return 1
	fi

	if [ -n "$USE_VALGRIND" ] ; then
		VALGRIND=$(command -v valgrind || true)

		if [ -z "$VALGRIND" ] ; then
			echo "USE_VALGRIND specified, but valgrind is not installed"
			exit 1
		else
			DETOX="$VALGRIND --quiet --error-exitcode=1 --track-origins=yes $DETOX"
		fi
	fi

	local BASE=/tmp/detoxtest/
	if [ ! -d $BASE ] ; then
		mkdir $BASE
	fi
	local WORK=$(realpath $(mktemp -d $BASE/test-XXXXXX))

	local SEQUENCE=unittest
	local RC=$WORK/detoxrc

	cat <<- DONE > $RC
	sequence $SEQUENCE {
	DONE

	while [ -n "$1" ] ; do

		case $1 in
			safe | iso8859_1 | utf_8 )
				local TABLE=$1.tbl
				if [ $1 = "utf_8" ] ; then
					TABLE="unicode.tbl"
				fi
				cp $TABLEPATH/$TABLE $WORK/$TABLE

				cat <<- DONE >> $RC
					$1 {
						filename "$WORK/$TABLE";
					};
				DONE
				;;

			lower | uncgi | wipeup )
				cat <<- DONE >> $RC
					$1;
				DONE
				;;

			wipeup-remove-trailing )
				cat <<- DONE >> $RC
					wipeup {
						remove_trailing;
					};
				DONE
				;;

			max-length-* )
				LENGTH=${1:11}
				cat <<- DONE >> $RC
					max_length {
						length $LENGTH;
					};
				DONE
				;;

			safe-search )
				cat <<- DONE >> $RC
					safe;
				DONE
				;;

			utf_8-search )
				cat <<- DONE >> $RC
					utf_8;
				DONE
				;;

			iso8859_1-search )
				cat <<- DONE >> $RC
					iso8859_1;
				DONE
				;;

			safe-basic )
				cat <<- DONE >> $RC
					safe {
						builtin "safe";
					};
				DONE
				;;

			utf_8-basic )
				cat <<- DONE >> $RC
					utf_8 {
						builtin "unicode";
					};
				DONE
				;;

			iso8859_1-basic )
				cat <<- DONE >> $RC
					iso8859_1 {
						builtin "iso8859_1";
					};
				DONE
				;;

			*)
				echo "Unknown method $1";
				return 1;
				;;
		esac

		shift
	done

	cat <<- DONE >> $RC
	};
	DONE

	cd $WORK

	local ACTION="touch"
	local OPERATOR="-f"
	local CMDINPUT="$INPUT"
	if [ "${INPUT: -1}" = "/" ] ; then
		ACTION="mkdir"
		OPERATOR="-d"
		CMDINPUT="${INPUT:0: -1}"
	fi

	$ACTION -- "$CMDINPUT"
	$DETOX -s $SEQUENCE -f $RC -- "$CMDINPUT"
	if [ ! $OPERATOR "$OUTPUT" ] ; then
		echo renaming "$INPUT" to "$OUTPUT" failed
		return 1
	fi

	return 0
}
