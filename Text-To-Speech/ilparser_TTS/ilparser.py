import os
import sys

if len(sys.argv)!=3:
	print 'pythpn ilparser.py <input-word> <lang_phonset_file>\n'
	sys.exit(1)

vow=[];
sym=[];

inp_word = sys.argv[1];
lang_file = open(sys.argv[2],'r')
out_file = open('wordpronunciation','w')

phoneset = []
for i in lang_file.readlines():
	st = i.strip().split()
	phn = st[1];
	if phn not in phoneset:
		phoneset.append(phn)
		if st[2]=='VOW':
			vow.append(phn)
		if st[2]=='SYM':
			sym.append(phn)

pronun="(set! wordstruct '( "

j=0;
syl='(('
while j<len(inp_word):
	stress = 1
	if inp_word[j:j+3] in phoneset:
		cur_phn = inp_word[j:j+3]
		j=j+3
	elif inp_word[j:j+2] in phoneset:
		cur_phn = inp_word[j:j+2]
		j=j+2
	else:
		if inp_word[j] in phoneset:
			cur_phn = inp_word[j]
			j=j+1
	syl = syl + '"' + cur_phn + '" '
	if cur_phn in vow:
		if(cur_phn=='a'):
			if(syl=='(("a" '):
				stress = 1;
			else:
				stress = 0;
		syl = syl + ') ' + str(stress) + ') '
		pronun = pronun + syl
		syl = '(('
	elif cur_phn in sym:
		new_pronun = pronun[0:len(pronun)-5] + syl[2:len(syl)] + pronun[len(pronun)-5:len(pronun)]
		pronun = new_pronun
		syl = '(('
		
if cur_phn not in vow:
	new_pronun = pronun[0:len(pronun)-5] + syl[2:len(syl)] + pronun[len(pronun)-5:len(pronun)]
	pronun = new_pronun

pronun = pronun + '))'
out_file.write(pronun)
