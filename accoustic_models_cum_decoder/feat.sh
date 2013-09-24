
FE=/home/fourier/nivedita/Tools/SPHINX/sphinxbase-0.6/build/bin/sphinx_fe

$FE -c etc/tel.ctl -di wav -do feats -ei wav -eo mfc -mswav yes -argfile models/hmm_telugu/feat.params

