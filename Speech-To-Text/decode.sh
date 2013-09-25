#!/bin/sh

$SPHINX3=/home/user/Tools/sphinx3

$SPHINX3/bin/sphinx3_decode \
-hmm ankur_tel_cd_cont_4000 \
-lm ankur_tel_lm_arpa.dmp \
-dict phone.dict \
-fdict filler.dict \
-ctl sample.ctl \
-cepdir feats \
-hyp sample_hyp.out \
-hypseg sample_hypseg.out


