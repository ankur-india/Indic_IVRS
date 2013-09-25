#!/bin/sh

$SPHINXTRAIN=/home/user/Tools/SphinxTrain

#Step 1:
mkdir hmm

#Step 2:
cd hmm

#Step 3:
$SPHINXTRAIN/scripts pl/setup SphinxTrain.pl -task tel

#The above script generates a series of folders in the hmm directory. The important directories are the:
#etc/ : which contains the conﬁguration ﬁles and the required transcript and dictionary ﬁles.
#wav/ : which contains tel raw audio data for training the models.
#feats/ : which contains the mfc ﬁles generated from the tel audio.
#model parameters/ : which contains our ﬁnal models obtained after the training purpose.
#model architecture/ : which contains the model deﬁnition ﬁle

#Download the AM_telugu directory from repository to home directory (Assuming this is done)

#Step 4:
cp ~/AM_telugu .

#Step 5:
cp AM_telugu/tel.dic AM_telugu/tel.filler AM_telugu/tel.phone etc/ 

#Step 6:
cp AM_telugu/tel_train.transcription etc

#Step 7:
cp ~/wav/* wav/

#Step 8:
ls wav/* > etc/tel_train.fileids

#Step 9:
perl scripts pl/make_feats.pl -ctl etc/tel_train.fileids

#Step 10:
perl scripts pl/RunAll.pl

echo "Acoustic Models are successfully created !!"
