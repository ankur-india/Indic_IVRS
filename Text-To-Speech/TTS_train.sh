#!/bin/sh

$FESTVOXDIR=/home/user/Tools/festvox

#Step 1: 
mkdir ankur_tel_indic

#Step 2: 
cd ankur_tel_indic

#Setting up directory structure

#Step 3: 
$FESTVOXDIR/src/clustergen/setup_cg ankur tel indic


#Before the setup, please download the following essential files for automatic labeling from here:
#https://github.com/bhavibond/Indic_IVRS/tree/master/essentials_ehmm

#Assuming that, you have already downloaded the required wav files from here:
#http://festvox.org/databases/iiit_voices/iiit_tel_baji.tar.gz

#Assuming you have already downloaded ehmm_essentials folder from wav from main repository

#Step 4: 
cp ~/Downloads/iiit_tel_baji/wav/* wav/

#Step 5: 
cp ~/Downloads/essentials_ehmm/etc/* etc/

#Step 6: 
cp ~/Downloads/essentials_ehmm/festvox/* festvox/

#Step 7: 
cp ~/Downloads/essentials_ehmm/bin/ilparser.py bin/

#Step 8: 
./bin/do_build build_prompts

#Step 9: 
./bin/do_build label

#Step 10:  
./bin/do_build build_utts

#Step 11:  
./bin/do_clustergen f0

#Step 12:  
./bin/do_clustergen mcep

#Step 13:  
./bin/do_clustergen voicing

#Step 14:  
./bin/do_clustergen combine_coeffs_v

#Step 15:  
./bin/do_clustergen generate_statenames

#Step 16:  
./bin/do_clustergen cluster

#Step 17: 
./bin/do_clustergen dur

echo "Text-To-Speech system successfully trained !!"
