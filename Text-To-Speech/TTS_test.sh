#!/bin/sh

#Assuming that you have already in the trained folder "Ankur_tel_indic" and all tools instaleld !!

#Step 1:
mkdir files

#Step 2:
echo -e "(voice_ankur_tel_indic_cg)\n(set! utt1 (utt.synth (Utterance Text \"raamudu maqchi baaludu\" ) ))\n(utt.save.wave utt1 \"files/sample_1.wav\")" >> files/sample.scm

#When the user clicks "Synthesize Speech" button, the back-end action should run the following program:

#Step 3:
festival festvox/ankur_tel_indic_cg.scm files/sample.scm

