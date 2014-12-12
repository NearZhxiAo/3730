#! /bin/sh

if [ "-name" = $1 ]; then
	FileName=$2 
	SrcWord=$3
	DstWord=$4
	echo Replace "$SrcWord" with "$DstWord" in the file "$FileName".
	find . -type f -name $FileName | xargs perl -i -pe s%"$SrcWord"%"$DstWord"%g
else
	SrcWord=$1
	DstWord=$2
	echo Replace "$SrcWord" with "$DstWord" in the whole file.
	find . -type f | xargs perl -i -pe s%"$SrcWord"%"$DstWord"%g
fi

