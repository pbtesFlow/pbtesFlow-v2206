#!/bin/sh
cd ${0%/*} || exit 1    # Run from this directory

logFilename=log.porousPimpleFoam
dataDir=energyDataDir
mkdir $dataDir
cp $logFilename $dataDir
logfile=$dataDir/$logFilename
#Reading force and torque calculated by functionObject integrating pressure over body
grep "^Hf = " $logfile | cut -d';' -f1 | cut -d'=' -f2 > $dataDir/Hf 
grep "^Hf = " $logfile | cut -d';' -f2 | cut -d'=' -f2 > $dataDir/Hs 
grep "^Hf = " $logfile | cut -d';' -f3 | cut -d'=' -f2 > $dataDir/Htot 
grep "^Time = " $logfile | cut -d' ' -f3 > $dataDir/t
