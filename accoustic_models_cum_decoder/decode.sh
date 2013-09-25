#!/bin/sh

SPHINX3=/home/fourier/nivedita/Tools/SPHINX/sphinx3-0.8/build/bin/sphinx3_decode

HMM=models/hmm_hp_v2
HMM1=models/hmm_telugu
LM=models/lm_telugu/tel_train.lm.DMP
LM1=models/lm_tourism/tourism.lm.DMP

$SPHINX3 -hmm ${HMM1} -lm ${LM1} -dict etc/tourism.dic -fdict etc/tel.filler -ctl etc/tel.ctl -logfn etc/log.txt -hyp etc/tel.txt -cepdir feats

