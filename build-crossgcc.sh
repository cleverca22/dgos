#!/bin/bash

set -x

quiet=0
scriptroot="$(cd "$(dirname "$0")" && pwd)"
patches="$scriptroot/patches/"
gnu_mirror="https://mirrors.kernel.org/gnu"
outdir=
archives=
prefixdir=
logfile=

# Set pipe errors to fail the command
set -o pipefail

function log() {
	if [[ $quiet = 0 ]]; then
		"$@" | tee -a "$logfile"
	else
		"$@" 2>&1 >> "$logfile"
	fi
}

function err() {
	local msg="$1"
	log echo "$msg"
	exit 1
}

function fullpath() {
	readlink -f "$1"
}

function require_cmd() {
	for command in "$@"; do
		if ! which "$command" > /dev/null; then
			err "$1 is required"
		fi
	done
}

function download_file() {
	local url="$1"
	local dest="$2"
		
	if [[ $url = "" ]] || [[ $dest = "" ]]; then
		err 'Missing argument to download_file'
	fi
	
	wget -P "$dest" -N "$url" || exit
}

function require_value() {
	local option="$1"
	local message="$2"
	
	if [[ "$option" = "" ]]; then
		err "$message"
	fi
}

function require_values() {
	local message="$1"
	shift
	for value in "$@"; do
		require_value "$value" "$message"
	done
}

function extract_tool() {
	local base="$1"
	local ext="$2"

	local input=$(fullpath "$base$ext")
	
	local name=${base#$archives/}
	
	echo "Extracting and configuring $name"
	
	local src="$outdir/src"
	local build="$outdir/build"	

	mkdir -p "$src" || exit
	mkdir -p "$build" || exit
	
	pushd "$src" || exit
	if ! [[ -d "$name" ]]; then
		tar xf "$input" || exit
	fi
	popd || exit
}

function make_tool() {
	local base="$1"
	local ext="$2"
	local target="$3"
	local config="$4"
	
	local name=${base#$archives/}
	
	local src="$outdir/src"
	local build="$outdir/build"	

	build=$(fullpath "$build")
	src=$(fullpath "$src")
	
	mkdir -p "$build/$name" || exit
	pushd "$build/$name" || exit
	log echo Making "$target" with config "$config" and prefix "$prefixdir" in $(pwd)...
	if ! [[ -f "Makefile" ]]; then
		log "$src/$name/configure" --prefix="$prefixdir" $config || exit
	fi
	log make $parallel $target || exit
	popd || exit
}

function process_tarball() {
	local archive="$1"
	local action="$2"
	local config="$3"
	local target="$4"
	
	local ext=
	local base=
	
	case "$archive" in
		*.tar.bz2) ext=".tar.bz2" base="${archive%%.tar.bz2}" ;;
		*.tar.gz)  ext=".tar.gz"  base="${archive%%.tar.gz}" ;;
		*)         err "Unrecognized archive extension" ;;
	esac
	
	log $action "$base" "$ext" "$config" "$target"
}

function help() {
	echo ' -a <arch list>   architectures to build (space separated list)'
	echo ' -m <url>         use specified GNU mirror'
	echo ' -j<#>            use <#> parallel workers to build'
	echo ' -o <dir>         output directory'
	echo ' -p <dir>         prefix dir'
	echo ' -q               quiet'
	echo ' -h|-?            display this help message'
	exit 1
}

# Parse arguments
while getopts a:m:j:o:p:qh? arg
do
    case $arg in
        a ) arches="$OPTARG" ;;
		m ) gnu_mirror="$OPTARG" ;;
        j ) parallel="-j$OPTARG" ;;
        o ) outdir="$OPTARG" ;;
		p ) prefixdir="$OPTARG" ;;
        q ) quiet=1 ;;
        h ) help ;;
        ? ) help ;;
        * ) echo "unrecognized option '$arg'" ; exit 1 ;;
    esac
done

logfile=$(fullpath "$outdir/build.log")
prefixdir=$(fullpath "$prefixdir")

require_value "$arches" "Architecture list required, need -a <dir>"
require_value "$outdir" "Output directory required, need -o <dir>"
require_value "$prefixdir" "Prefix directory required, need -p <dir>"
require_value "$gnu_mirror" "Specified mirror URL is invalid: -m $gnu_mirror"

require_cmd "wget" "tar" "g++" "gcc" "as" "ar" "ranlib" "nm" "tee" "tail"

# Set gccver, binver, etc...
. "$scriptroot/build-crossgcc-versions"

require_values "build-crossgcc-versions is file invalid" \
	"$gccver" "$binver" "$gdbver" "$gmpver" "$mpcver" "$mpfver"

gcctar="gcc-$gccver.tar.gz"
bintar="binutils-$binver.tar.bz2"
gdbtar="gdb-$gdbver.tar.gz"
gmptar="gmp-$gmpver.tar.bz2"
mpctar="mpc-$mpcver.tar.gz"
mpftar="mpfr-$mpfver.tar.bz2"

gccurl="$gnu_mirror/gcc/gcc-$gccver/$gcctar"
binurl="$gnu_mirror/binutils/$bintar"
gdburl="$gnu_mirror/gdb/$gdbtar"
gmpurl="$gnu_mirror/gmp/$gmptar"
mpcurl="$gnu_mirror/mpc/$mpctar"
mpfurl="$gnu_mirror/mpfr/$mpftar"

archives="$outdir/archives"
mkdir -p "$archives" || exit
download_file "$gccurl" "$archives"
download_file "$binurl" "$archives"
download_file "$gdburl" "$archives"
download_file "$gmpurl" "$archives"
download_file "$mpcurl" "$archives"
download_file "$mpfurl" "$archives"

for tarball in $archives/*.tar.*; do
	log echo Extracting tarball $tarball
	process_tarball "$tarball" "extract_tool"
done

ln -sf $(fullpath "$outdir/src/gmp-$gmpver") \
	$(fullpath "$outdir/src/gcc-$gccver/gmp")
ln -sf $(fullpath "$outdir/src/mpc-$mpcver") \
	$(fullpath "$outdir/src/gcc-$gccver/mpc")
ln -sf $(fullpath "$outdir/src/mpfr-$mpfver") \
	$(fullpath "$outdir/src/gcc-$gccver/mpfr")

gcc_config="--target=$arches --with-system-zlib \
--enable-multilib --enable-languages=c,c++ \
--enable-shared --enable-system-zlib --enable-threads"

bin_config="--target=$arches --enable-gold --enable-ld"
gdb_config="--target=$arches"

process_tarball "$archives/$bintar" "make_tool" all "$bin_config"
process_tarball "$archives/$gdbtar" "make_tool" all "$gdb_config"
for target in all-gcc all-target-libgcc install-gcc install-target-libgcc; do
	process_tarball "$archives/$gcctar" "make_tool" "$target" "$gcc_config"
done

process_tarball "$archives/$bintar" "make_tool" install "$bin_config" 
process_tarball "$archives/$gdbtar" "make_tool" install "$gdb_config" 