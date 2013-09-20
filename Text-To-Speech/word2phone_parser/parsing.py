import os
import sys

if len(sys.argv)!=4:
	print 'pythpn parsing.py <input-txt> <output-phones> <Lang-phoneset>\n'
	sys.exit(1)

in_file = open(sys.argv[1],'r')
out_file = open(sys.argv[2],'w')

lang_file = open(sys.argv[3],'r')

phoneset = []
for i in lang_file.readlines():
	st = i.strip().split()
	phn = st[1];
	if phn not in phoneset:
		phoneset.append(phn)

for i in in_file.readlines():
	st = i.strip().split()
	out_file.write(st[1]+'\n')
	count=3;
	while count<len(st)-1:
		word= st[count]
#		print word+' '+str(len(word))
		count=count+1
		j=0;
		while j<len(word):
			if word[j:j+3] in phoneset:
#				print word[j:j+3]
				out_file.write(word[j:j+3]+'\n')
				j=j+3
			elif word[j:j+2] in phoneset:
#				print word[j:j+2]
				out_file.write(word[j:j+2]+'\n')
				j=j+2
			else:
				if word[j] in phoneset:
#					print word[j]
					out_file.write(word[j]+'\n')
					j=j+1
