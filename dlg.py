#encoding: utf-8
'''
mecanisme de dialogues : stocker en RAM les dialogues : 

representation binaire
list of 16 absolute u16 start offsets + data


i16 : nb de reponses (>0)
pascal string (0-255 chars)
(suivi de )
i16 (absolute offset to nextanswer) - reponse 1
offset2-reponse 2
offset3-reponse 3

@offset1 : pnj1- nb2 - reponse 
puis réponses ...
 special offsets : negative ones.

 etc etc 

representation texte : 

intro:pnj:texte texte texte texte ou intro:texte (pas de perso)
        oui -> label1
        ah bah non -> label2

label1: bon ok
        ok -> intro

label2 : vtff
        ok -> exit_1

+ fin dialogue avec @verb = negative offsets
exit_1 = -1
exit_2 = -2
etc

'''

debug = False

from collections import OrderedDict
from StringIO import StringIO
import struct, sys, zlib

# cut into several dialogs, produce one list of lines per dialog
# skips empty lines and comment lines
def all_dialogs() :        
        dialog=[]
        for l in open(sys.argv[1]) :
                if not l.strip() : continue
                if l.strip().startswith ('#') : continue
                if l.startswith("===") :
                        yield dialog
                        dialog=[]
                else : 
                        dialog.append(l)
        if dialog : yield dialog

# parsing to request:[responses]
def encode_dialog(dialog_lines,of) : 
        dlg = OrderedDict()
        cur_q = None
        for l in dialog_lines : 
                if l.startswith(' ') or l.startswith('\t') : 
                        # response
                        s,label = l.split('->',1)
                        dlg[cur_q][1][label.strip()] = s.strip()
                else : 
                        label, text = l.split(':',1)
                        cur_q = label.strip()
                        dlg[cur_q] = text.strip(),OrderedDict()
        # test it ?  check that each answer exists

        # record offsets for each label
        offsets={}
        cur_ofs=of.tell()
        for lbl,(txt,d) in dlg.items() :        
                offsets[lbl] = cur_ofs 
                cur_ofs += 3+len(txt)
                for _,s in d.items() : 
                        cur_ofs += 3+len(s)

        # now output 
        for lbl,(txt,d) in dlg.items() : 
                if debug: print offsets[lbl],lbl,txt,len(d)
                of.write(struct.pack("hB",len(d),len(txt)))
                of.write(txt)
                for tgt,s in d.items() :
                        ofs = -int(tgt[5:]) if tgt.startswith('exit_') else offsets[tgt]
                        if debug: print '    ',tgt,ofs,s
                        of.write(struct.pack("hB",ofs,len(s)))
                        of.write(s)


of = StringIO()
starts = []
for dialog in all_dialogs() :
        starts.append(of.tell())
        encode_dialog(dialog, of)

print len(starts),"dialogs :",starts
out_data = zlib.compress(of.getvalue(),9)
print len(of.getvalue()), ' zips to', len(out_data)

base_name = sys.argv[1].rsplit('.',1)[0]
open(base_name+'.dlz','w').write(out_data)

# testing dialogs
'''

inf = of
def read_couple() : 
        nb,ls = struct.unpack('hB',inf.read(3))
        s=inf.read(ls)
        return nb,s

print '-'*80


for ofs in starts :
        print " == starting next dialog"
        while ofs>=0 : 
                inf.seek(ofs)
                nb,s = read_couple()
                answ = [read_couple() for i in range(nb)]

                print s
                print "--"
                for i,(ofs,s) in enumerate(answ) : 
                        print i,s
                print "--"
                while True :
                        x=input()
                        if x < len(answ) : break
                
                ofs = answ[x][0]
                
        print "result",-ofs

'''



