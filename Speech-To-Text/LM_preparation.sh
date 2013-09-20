#!/bin/sh

$CMULM=/home/user/Tools/cmuclmtk
$SPHINXDIR=/home/user/Tools/sphinx3

# Step 1:
mkdir lm_telugu

#Step 2: 
cp tel_train.transcription lm_telugu/

#Step 3: 
cd lm_telugu

#Step 4: 
echo -e "<s>\n</s>" >> tel.ccs

#The following are the steps to generate the language model using LMTK:

#Step 5: 
cat tel_train.transcription | $CMULM/bin/text2wfreq > tel_train.wfreq

#Step 6: 
cat tel_train.wfreq | $CMULM/bin/wfreq2vocab > tel.vocab

#Step 7: 
cat tel_train.transcription | $CMULM/bin/text2idngram -n 3 -vocab tel.vocab -idngram tel_train.idngram

#Step 8: 
$CMULM/bin/idngram2lm -n 3 -vocab tel.vocab -idngram tel_train.idngram -arpa tel_train.lm -context an4.ccs

#For decoding we have used the language model dump ﬁle. We can obtain the
#dump ﬁle from the tel_train.lm ﬁle using the sphinx3 script sphinx_lm_convert as shown below:

#Step 9: 
$SPHINXDIR/bin/sphinx3_lm_convert -i tel_train.lm -o tel_train.lm.DMP

echo "Language Models are successfully created !!"
