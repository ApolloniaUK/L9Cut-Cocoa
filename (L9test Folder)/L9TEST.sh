#!/bin/bash
echo "***DETECT & REFUSE V1 DATA**************"
./l9cut ../../L9test\ Folder/COLAD.T64 zzz.tmp
echo "***CLEAN DETECT V2**********************"
./l9cut ../../L9test\ Folder/SNOW_P.V2 zzz.tmp
echo "***V3 DETECT/DESPERATE (SHORT FILE)*****"
./l9cut ../../L9test\ Folder/MOLE1_B.V3 zzz.tmp
echo "****************************************"
#pause
echo "***CLEAN V3 DETECT**********************"
./l9cut ../../L9test\ Folder/MOLE1_P.V3 zzz.tmp
echo "***CORRUPT VERSION OF SAME FILE*********"
./l9cut ../../L9test\ Folder/MOLE1_P.V3_ zzz.tmp
echo "****************************************"
#pause
echo "***CLEAN V4*****************************"
./l9cut ../../L9test\ Folder/SCAPE3_M.V4 zzz.tmp
echo "***SPLIT V4 (END OF DATABASE!)**********"
./l9cut ../../L9test\ Folder/RAJ2.FR zzz.tmp
echo "***Z80 DECOMP & CLEAN V3 & DEPROTECT****"
./l9cut ../../L9test\ Folder/WORM.Z80 zzz.tmp
echo "****************************************"
#pause
echo "***SPLIT V3 & WARNING MESSAGE***********"
./l9cut ../../L9test\ Folder/REDM128.Z80 zzz.tmp
echo "***TEST FOR 64K MEMORY SPACE************"
./l9cut ../../L9test\ Folder/TEST64 zzz.tmp
echo "***TEST FOR 100K (NOT W/DOS VERSION!)***"
./l9cut ../../L9test\ Folder/TEST100 zzz.tmp
echo "****************************************"
#rm ../../L9test\ Folder/*.dmp
