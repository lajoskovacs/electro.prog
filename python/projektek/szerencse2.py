#!/usr/bin/python3
# -*- coding: utf-8 -*-
# szerencse 2.0

import os
import random
    

#******************************************************************************
# 	véletlen számok generálása  (1 és "numbermax"  között)
#	nem lehet  ismétlődés !!!
#	tip -->  hány darab szám kell (pl. 5-ös lottó esetén 5)
# 	numbermax -->  tartomány vége, legnagyobb szám 
#						(pl. 5-ös lottó esetén 90)
#******************************************************************************
def randomTip(tip,numbermax):
    number=0   # egy új szám tárolására
    isIt=True   # volt-e már a szám
    ltip1=[]   # lista  véletlen számok tárolására 
    if numbermax<tip:
        print(" valami gond van a paraméterekkel !! 'tip' túl nagy !!")
    else:
        while len(ltip1)<tip:        # lista feltöltése
            isIt=True
            while isIt:   # egy új véletlen szám keresése, ami még nem volt
                number=random.randint(1,numbermax)   #  1 és "numbermax" közötti véletlen szám
                isIt=number in ltip1    # volt-e már ez a szám ?
            ltip1.append(number)  # meg van az új szám -> bele a listába
    return ltip1


#******************************************************************************
# 	számok véletlen kiválasztása  a megadottak közül
#	tip -->  hány darab szám kell 
# 	lnumbers -->  a számok, amelyek közül választani lehet, lista !
#******************************************************************************
def randfromNumbers(tip,lnumbers):
    number=0   # egy új szám tárolására
    lrandom=[] # lista véletlen számokhoz
    ltip1=[]   # lista  a kiválasztott számok tárolására 
    size=len(lnumbers)
    if size<tip:
        print(" valami gond van a paraméterekkel !! 'tip' túl nagy !!")
    else:
        lrandom=randomTip(tip,size)
        for i in lrandom:        
            number=lnumbers[i-1]    # a kiválasztott számok beírása
            ltip1.append(number)    # a kimeneti listába
    return ltip1

 

#******************************************************************************
# 	szám kombinációkat állít elő a megadott számokból
#	tip -->  hány darab szám kell egy kombinációhoz
# 	lnumbers -->  a számok, amelyek közül választani lehet, lista !
#******************************************************************************
def combinationTip(tip,lnumbers):
    ltip1=[]   # lista  egy tipp tárolására 
    ltips=[]   # listák listája  sok tipp tárolására    
    number=0   # egy új szám tárolására    
    index=[]   # ?
    size=0  # lnumbers lista mérete
    generate=True   # ?
    size=len(lnumbers)
    if size<tip:
        print(" valami gond van a paraméterekkel !! 'tip' túl nagy !!")
        return ltips
    else:
        index=list(range(tip))
        print(" Kombinációk készítése!")
        cycl=1
        while generate:
            ltip1.clear()
            #print(" index:  ",index)
            for i in tuple(range(tip)):  # egy új kombináció előállítása
                number=lnumbers[index[i]]
                ltip1.append(number)
            if cycl%10==0:
                print("*",end='')
            #print(" ltip1:  ",ltip1)
            ltips.append(list(ltip1))  # egy új kombináció felvétele
            #print(" ltips:  ",ltips)
            cycl+=1
            if index[tip-1]<(size-1):
                index[tip-1]+=1    # az utolsó index léptetése, ha lehet
            else:   # előző indexek léptetése
                lep=tip-1
                keres=True
                while keres:
                    if lep>0:
                        lep-=1
                        if index[lep+1]-index[lep]>1:
                            keres=False
                    else:  # vége a mókának :) 
                        keres=False
                        generate=False
                if generate:
                    index[lep]+=1
                    for i in tuple(range(lep+1,tip)):
                        index[i]=index[i-1]+1
        print("\n")
        print(" kész ")
        print(" Kombinációk (tippek) száma:  ",len(ltips))
        return ltips    
       

#******************************************************************************
# 	hibapontos kombinációt állít elő 
#    a megadott kombinációból ("megszűri" a kombinációt)    
#	fault -->  hiba szám (mennyi eltérés lehet)
# 	ltips -->  bemenő  számsorok (tippek), amelyek közül választani kell (szűrni kell)
#******************************************************************************
def filtercombinationTip(fault,ltips):   
    lfaulttips=[]   # listák listája, az eredmény (megszűrt) tippsorok 
    lfiltered=[]   # egy tipsor tárolása, munka lista
    diff=0    #  eltérő számok  száma
    tipsize=0   #  tippsorok száma
    filteredsize=0  #  szűrt tippsorok száma
    OK=True
    tipsize=len(ltips)
    print(" Hibapontos tippek készítése")
    lfaulttips.append(list(ltips[tipsize-1]))  # az utolsó kombináció felvétele a szűrtek közé
    for tip1 in ltips:  # végignézzük az összes tippet (kombinációt)
        OK=True
        i=0
        filteredsize=len(lfaulttips)
        while (i<filteredsize) and OK:   # összevetjük a már megszűrt tippekkel
            diff=0
            lfiltered=list(lfaulttips[i])
            for key in tip1:   # megszámoljuk mennyi szám az eltérés
                if key not in lfiltered:
                    diff+=1
            if diff<=fault:   # kicsi az eltérés --> kiszűrjük !!
                OK=False
            i+=1
        if OK:     # a kombináció felvétele a szűrtek közé
            lfaulttips.append(list(tip1))  
    filteredsize=len(lfaulttips)
    print(" Hibapontos tippek száma:  ",filteredsize)
    # bem=input('\n')
    return lfaulttips


#******************************************************************************
def teszt1(): 
    ltip=[]        # lista a tipphez
    lnum=[]       #  lista a számokhoz 
    lnum=randomTip(8,45)
    print(" Számok:  ",end=' ')
    print(lnum)
    print(' \n')
    ltip=combinationTip(6,lnum)
    print(" Kombinációk:  ")
    for tip in ltip:
        print(tip)
    lfilted=filtercombinationTip(1,ltip)
    print(" Hibapontos kombinációk:  ")
    for tip2 in lfilted:
        print(tip2)        
    bem=input('\n')
  
  
#******************************************************************************
def comb(size,number):
    #  combination
    #  pl.  6-os lottó:  45 számból 6-ot húznak, kombináció szám -->  45!/((45-6)! * 6!)   -->
    #   combi= 45*44*43*42*41*40/(1*2*3*4*5*6) 
    combi=1
    x=size
    for i in tuple(range(number)):
        combi*=x   # kombináció    size!/((size-number)! * number!)
        x-=1
    for i in tuple(range(number)):
        combi/=(i+1)   
    print(size," számból -> ",number,"  szám")
    print(" esély a telitalálatra kombináció nélkül (1db tippel)->   1 : ",combi)    
                

#******************************************************************************
def comb2(size,number1,number2):
    #combination2
	# pl.  kenó:  80 számból 20-at húznak, kombináció szám -->  80!/((80-20)! * 20!)   -->
	#			combi1= (80*79*78*77*76*75*74*73*72*71*70*69*68*67*66*65*64*63*62*61)/(1*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20)
	# ebből  10-es játék esetén jó kombinációk (telitalálat)  --> (80-10)!/((80-(20-10))! * (20-10)!)
	#			combi2= (70*69*68*67*66*65*64*63*62*61)/(1*2*3*4*5*6*7*8*9*10)
	# esély a 10-es telitalálatra -->    1 / (combi1/combi2)
    combi1=1
    combi2=1
    x=size
    for i in tuple(range(number1)):
        combi1*=x   # kombináció    size!/((size-number1)! * number1!)
        x-=1
    for i in tuple(range(number1)):
        combi1/=(i+1)   
    x=size-number2
    for i in tuple(range(number1-number2)):
        combi2*=x   # kombináció 2.    (size-number2)!/((size-(number1-number2))! * (number1-number2)!)
        x-=1
    for i in tuple(range(number1-number2)):
        combi2/=(i+1)         
    print(size," számból -> ",number1,"  szám húzva ->  ",number2,"  szám eltalálva")
    print(" esély a telitalálatra kombináció nélkül (1db tippel)->   1 : ",combi1/combi2)    
  

#******************************************************************************
def lotto(size,number): 
    lnum=[]       #  lista a számokhoz 
    ltips=[]        # listák listája, a  tippsorokhoz  
    lfaulttips=[]   # listák listája, az eredmény (megszűrt) tippsorok 
    letsgo=False
    combi=0     # combination, mennyi számból csinuljunk kombinációs tippet
    faults=0    # hibapont, mennyi lehet az eltérés (a hiba)
    while not letsgo:
        print(" ")
        print( "Mennyi számot kombináljunk ? Válassz ! ",number, " - ",size)
        text1=str(input())
        try:
            combi=int(text1)
            if combi>=number and combi<=size:
                letsgo=True
            else:
                print("Nem jó szám !! Próbáld újra ")
        except:
            combi=number
            print("Nem  szám !! Próbáld újra ")
    lnum=randomTip(combi,size)
    print(" ")
    print('A véletlen számok: ')
    for i in lnum:
        print(i," ",end=' ')
    print(" ")
    if fajlOK:
        fajl_ki.write(" Ennyi db számot kombinálunk:  ")
        fajl_ki.write(str(combi))
        fajl_ki.write('\n')  
        fajl_ki.write(" A véletlen számok:  ")
        fajl_ki.write(str(lnum))
        fajl_ki.write('\n') 
    ltips=combinationTip(number,lnum)       # az összes variáció előállítása
    letsgo=False
    print(" ")
    while not letsgo:
        print( "Hibapontok száma ? Válassz ! (0 - ",number, ")")
        text1=str(input())
        try:
            faults=int(text1)
            if faults>=0 and faults<=number:
                letsgo=True
            else:
                print("Nem jó szám !! Próbáld újra ")
        except:
            faults=0
            print("Nem  szám !! Próbáld újra ")    
    lfaulttips=filtercombinationTip(faults,ltips)       # szűrés !! hibapontos kombináció készítése
    print(" ")
    print(" Hibapontos kombinációk:  ")
    for i in lfaulttips:
        print(i)
    if fajlOK:
        fajl_ki.write(" hibapontok száma:  ")
        fajl_ki.write(str(faults))
        fajl_ki.write('\n')  
        fajl_ki.write(" Hibapontos kombinációk:  ")
        fajl_ki.write('\n') 
        for i in lfaulttips:
            fajl_ki.write(str(i)) 
            fajl_ki.write('\n')   
    print(" ")         
    print("Nyomj meg egy billentyűt ! ")
    print("  ")
    print("  ")
    text1=str(input())   #  várakozás billentyűnyomásra


#******************************************************************************
def lotto5(): 
    print("  ")
    print("  ")
    print(" Lottó 5  ")
    if fajlOK:
        fajl_ki.write(" Lottó 5  ")
        fajl_ki.write('\n') 
    comb(90,5)
    lotto(90,5)
    
    
#******************************************************************************
def lotto6(): 
    print("  ")
    print("  ")
    print(" Lottó 6  ")
    if fajlOK:
        fajl_ki.write(" Lottó 6  ")
        fajl_ki.write('\n') 
    comb(45,6)
    lotto(45,6)
    
       
#******************************************************************************
def lotto7(): 
    print("  ")
    print("  ")
    print(" Lottó 7 (Skandináv lottó)  ")
    if fajlOK:
        fajl_ki.write(" Lottó 7  ")
        fajl_ki.write('\n') 
    comb(35,7)
    lotto(35,7)
    
      
#******************************************************************************
def putto(): 
    print("  ")
    print("  ")
    print(" Puttó  ")
    if fajlOK:
        fajl_ki.write(" Puttó  ") 
        fajl_ki.write('\n')    
    comb(20,8)
    lotto(20,8)
    
       
#******************************************************************************
def eujp(): 
    print("  ")
    print("  ")
    print(" Euro Jackpot ")
    if fajlOK:
        fajl_ki.write(" Euro Jackpot  ")
        fajl_ki.write('\n')     
    comb(50,5)
    lotto(50,5)
    
      
#******************************************************************************
def keno(): 
    number=10
    letsgo=False

    print(" Kenó ")
    print("  ")
    while not letsgo:
        print( "Milyen Kenó legyen ? Válassz ! (2 - 10)")
        text1=str(input())
        try:
            number=int(text1)
            if number>=2 and number<=10:
                letsgo=True
            else:
                print("Nem jó szám !! Próbáld újra ")
        except:
            number=10
            print("Nem  szám !! Próbáld újra ")      
    print("  ")
    print(" Kenó ",number)
    if fajlOK:
        fajl_ki.write(" Kenó  ")
        fajl_ki.write(str(number))
        fajl_ki.write('\n')     
    comb2(80,20,number)
    lotto(80,number)
    
    
#******************************************************************************
def odds(): 
    lotto5comb= 90*89*88*87*86/(2*3*4*5)
    lotto6comb= 45*44*43*42*41*40/(2*3*4*5*6)
    lotto7comb= 35*34*33*32*31*30*29/(2*3*4*5*6*7)
    eujp5comb= 50*49*48*47*46/(2*3*4*5)
    putto8comb= 20*19*18*17*16*15*14*13/(2*3*4*5*6*7*8)
    keno10comb= 79*5*74*73*71/17	    # (80*79*78*77*76*75*74*73*72*71)/(11*12*13*14*15*16*17*18*19*20)
    keno9comb= 79*11*5*74*73/17	        #  (80*79*78*77*76*75*74*73*72)/(12*13*14*15*16*17*18*19*20) 
    keno8comb= 79*11*5*37*73/(17*3)	    #  (80*79*78*77*76*75*74*73)/(13*14*15*16*17*18*19*20) 
    keno7comb= 79*13*11*5*37/(17*3) 	#  (80*79*78*77*76*75*74)/(14*15*16*17*18*19*20) 
    keno6comb= 79*26*77*5/(17*6)	    #  (80*79*78*77*76*75)/(15*16*17*18*19*20) 
    keno5comb= 79*13*77/(17*3)	        #  (80*79*78*77*76)/(16*17*18*19*20) 
    keno4comb= 79*13*77*4/(17*3*19) 	# (80*79*78*77)/(17*18*19*20)
    keno3comb= 79*13*4/(3*19)	        # (80*79*78)/(18*19*20)    
    print("Helló ")
    print(" lottó5 esély 	     -->  1:  ",lotto5comb)
    print(" lottó6 esély 	     -->  1:  ",lotto6comb) 
    print(" lottó7 esély 	     -->  1:  ",lotto7comb)    
    print(" euro jackpot esély 	 -->  1:  ",eujp5comb)    
    print(" puttó esély 	     -->  1:  ",putto8comb) 
    print(" kenó 10 esély 	     -->  1:  ",keno10comb) 
    print(" kenó 9 esély 	     -->  1:  ",keno9comb) 
    print(" kenó 8 esély 	     -->  1:  ",keno8comb) 
    print(" kenó 7 esély 	     -->  1:  ",keno7comb) 
    print(" kenó 6 esély 	     -->  1:  ",keno6comb) 
    print(" kenó 5 esély 	     -->  1:  ",keno5comb) 
    print(" kenó 4 esély 	     -->  1:  ",keno4comb) 
    print(" kenó 3 esély 	     -->  1:  ",keno3comb)  
    print("  ")
    print("Nyomj meg egy billentyűt ! ")
    print("  ")
    print("  ")
    text1=str(input())   #  várakozás billentyűnyomásra  
    
    
# *********************************  FŐ PROGRAM  **********************************

letsgo=True
menu='1'
fajlOK=False

# ********   start    

try:
    fajl_ki=open('tippek.txt','w')
    fajlOK=True
except IOError:
    print('Nem sikerült fájlt megnyitni!')
    
while letsgo:
    print("************************************************")
    print("*                                              *")
    print("*  Lottó5 - 5    Lottó6 - 6    Lottó7 - 7      *")
    print("*                                              *")
    print("*  Puttó  - 8          Kenó   - 9              *")
    print("*                                              *")
    print("*  Euro JackPot - 4        Esélyek  - 3        *") 
    print("*                                              *")
    print("*  Vége   - 1                                  *")    
    print("*                                              *")    
    print("************************************************")    
    print("  ")
    print("  ") 
    print("Válassz egy számot !!")
    try:
        text1=str(input())
        menu=text1[0]
    except:
        menu='e'
    if menu=='1':
        letsgo=False     # vége, kilépés
    if menu=='5':
        lotto5()
    elif menu=='6':
        lotto6()
    elif menu=='7':
        lotto7()
    elif menu=='8':
        putto()
    elif menu=='9':
        keno()
    elif menu=='4':
        eujp()
    elif menu=='3':
        odds()
    else:
        print("Nem jó szám !! Próbáld újra ")
        print("  ")
        print("  ")

# ******************* lezárás ***********************************
if fajlOK:
	fajl_ki.close()

#****************  itt a vége! ******************************* 


    
