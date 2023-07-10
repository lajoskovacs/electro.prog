# Lottó, Kenó, Puttó ...
# számolgatások, statisztikák
# 2022.03.23 - 2022.04.08    KL

# import os
# os.environ['KIVY_NO_CONSOLELOG']='1'

from kivy.config import Config 
Config.set('graphics','width','640')
Config.set('graphics','height','1000')
# Config.set('graphics','resizable','0')

from kivy.app import App
from kivy.uix.button import  Button
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.spinner import Spinner
from kivy.uix.slider import Slider
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.widget import Widget
from kivy.uix.popup import Popup
from kivy.properties import ObjectProperty
from kivy.lang import Builder
import os
import random
    
# font_size: self.width/20
# font_size: '15sp'
	
Builder.load_string('''

<LuckyWidget>:
    sp1jatek: sp1jatek
    sl1szamdb: sl1szamdb
    lab3szamdb: lab3szamdb
    text1szamok: text1szamok
    text2tippek: text2tippek
    sl2hiba: sl2hiba

	TextInput:				
		text: 'Lottó, Kenó, Puttó'
		pos_hint: {'top':1}
        size_hint: 1, 1/10		
		font_size: self.width/10
        foreground_color:1,0,0,1
        background_color:1,1,0,1
        multiline: False
        readonly: True
        
	Label:				
		text: 'Játék'
		pos_hint: {'top':18/20}
        size_hint: 1/3, 1/20	
		font_size: self.width/8         
        
    Spinner: 
        id: sp1jatek
        text: "Kenó10"
		pos_hint: {'top':17/20}
        size_hint: 1/3, 1/20	        
        values:'Lottó 5','Lottó 6','Lottó 7','Euro JP','Puttó','Kenó10','Kenó9','Kenó8','Kenó7','Kenó6','Kenó5','Kenó4'
		font_size: self.width/8
        on_text: root.selectgame()
        
	Label:				
		text: 'Mennyi szám?'
		pos_hint: {'right':1,'top':18/20}
        size_hint: 1/2, 1/20	
		font_size: self.width/10   
        
    Slider: 
        id: sl1szamdb
		pos_hint: {'right':1,'top':17/20}
        size_hint: 1/2, 1/20       
        min: 10
        max: 18
        value: 10
        step: 1
        on_value: root.selectnumber()
  
	Label:		
        id: lab3szamdb
		text: str(sl1szamdb.value)
		pos_hint: {'right':9/10,'top':16/20}
        size_hint: 1/6, 1/20	
		font_size: self.width/4          
     
	Button:				
		text: 'véletlen számok'
		pos_hint: {'top':15/20}
        size_hint: 9/20, 1/20	
		font_size: self.width/10  
        on_press: root.randomTip() 
		     
	TextInput:	
        id: text1szamok
		text: ' '
		pos_hint: {'top':14/20}
        size_hint: 7/10, 2/15		
		font_size: self.width/15
        foreground_color:1,0,0,1
        background_color:0,0,0.5,1
        multiline: True
        readonly: True
        
	Label:				
		text: 'Hibapont'
		pos_hint: {'right':1,'top':14/20}
        size_hint: 1/4, 1/20	
		font_size: self.width/5      
        
    Slider: 
        id: sl2hiba
		pos_hint: {'right':1,'top':13/20}
        size_hint: 1/4, 1/20        
        min: 0
        max: 5
        value: 0
        step: 1
        on_value: root.selecthiba()
  
	Label:		
        id: lab4hiba
		text: str(sl2hiba.value)
		pos_hint: {'right':9/10,'top':12/20}
        size_hint: 1/8, 1/20	
		font_size: self.width/3            
        
        
	Button:				
		text: 'Hibapontos kombináció'
		pos_hint: {'top':11/20}
        size_hint: 6/10, 1/20	
		font_size: self.width/15  
        on_press: root.combinationTip() 
		     
	TextInput:	
        id: text2tippek
		text: ' '
		pos_hint: {'top':10/20}
        size_hint: 1, 5/10		
		font_size: self.width/20
        foreground_color:1,0,0,1
        background_color:0,0,0.5,1
        multiline: True
        readonly: False                                          
		                                                          
''')



class LuckyWidget(FloatLayout):
	

    #******************************************************************************	
    def __init__(self):
        super().__init__()
        self.minszam= 10    # minimum hány számot játszunk meg, a játéktípusa (alap Kenó 10)
        self.maxszam= 10    # (tip) maximum hány számot játszunk meg (max. 20, alapból = minszam)   
        self.szamtart= 80    # (numbermax) hány számból húznak (pl. Lottó5-nél 90, Kenó10-nél 80)  
        self.szamok=[]    	# a megjátszandó számok
        self.tippek=[]      # a kombinációs tippek
        self.hibatippek=[]      # a hibapontos  kombinációs tippek
        self.hiba= 0        # hibapont



    #******************************************************************************
    def selectgame(self):
        print(self.sp1jatek.text)
        if self.sp1jatek.text=='Lottó 5':
            self.minszam=5
            self.maxszam=5
            self.szamtart= 90   
        elif self.sp1jatek.text=='Lottó 6':
            self.minszam=6
            self.maxszam=6  
            self.szamtart= 45          
        elif self.sp1jatek.text=='Lottó 7':
            self.minszam=7
            self.maxszam=7  
            self.szamtart= 35          
        elif self.sp1jatek.text=='Puttó':
            self.minszam=8
            self.maxszam=8
            self.szamtart= 20
        elif self.sp1jatek.text=='Euro JP':
            self.minszam=5
            self.maxszam=5 
            self.szamtart= 50  
        elif self.sp1jatek.text=='Kenó10':
            self.minszam=10
            self.maxszam=10 
            self.szamtart= 80  
        elif self.sp1jatek.text=='Kenó9':
            self.minszam=9
            self.maxszam=9 
            self.szamtart= 80 
        elif self.sp1jatek.text=='Kenó8':
            self.minszam=8
            self.maxszam=8 
            self.szamtart= 80  
        elif self.sp1jatek.text=='Kenó7':
            self.minszam=7
            self.maxszam=7 
            self.szamtart= 80             
        elif self.sp1jatek.text=='Kenó6':
            self.minszam=6
            self.maxszam=6 
            self.szamtart= 80  
        elif self.sp1jatek.text=='Kenó5':
            self.minszam=5
            self.maxszam=5 
            self.szamtart= 80
        elif self.sp1jatek.text=='Kenó4':
            self.minszam=4
            self.maxszam=4 
            self.szamtart= 80                                                                                 
                    
        self.sl1szamdb.min=self.minszam     # szám db választó slider minimális értékének módosítása
        self.sl1szamdb.value=self.minszam	# szám db választó slider aktuális értékének módosítása
        self.text1szamok.text= ' '          # az előzőleg sorsolt számok törlése
        self.szamok.clear()


    #******************************************************************************
    def selectnumber(self):
        
        self.maxszam=self.sl1szamdb.value       # kisorsolandó számok darabszámának választása
        self.text1szamok.text= ' '              # az előzőleg sorsolt számok törlése
        self.szamok.clear()
        print(self.maxszam)


    #******************************************************************************
    def selecthiba(self):
        self.hiba=self.sl2hiba.value       # hibaszám választása
        print(self.hiba)
        
  

    #******************************************************************************   
         
    def comb(self,size,number):
        """ pl.  6-os lottó:  45 számból 6-ot húznak, kombináció szám -->  
        45!/((45-6)! * 6!)   -->  combi= 45*44*43*42*41*40/(1*2*3*4*5*6)  """
        
        combi=1
        x=size
        for i in tuple(range(number)):
            combi*=x   # kombináció    size!/((size-number)! * number!)
            x-=1
        for i in tuple(range(number)):
            combi/=(i+1)
        return combi    # ods
   
                

    #******************************************************************************
	
    def comb2(self,size,number1,number2):
        ''' pl.  kenó:  80 számból 20-at húznak, kombináció szám -->  80!/((80-20)! * 20!)  
        combi1= (80*79*78*77*76*75*74*73*72*71*70*69*68*67*66*65*64*63*62*61)/
        (1*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20)
        ebből  10-es játék esetén jó kombinációk (telitalálat)  --> 
        (80-10)!/((80-(20-10))! * (20-10)!)
        combi2= (70*69*68*67*66*65*64*63*62*61)/(1*2*3*4*5*6*7*8*9*10)
        esély a 10-es telitalálatra -->    1 / (combi1/combi2) '''
        
        combi1=1
        combi2=1
        x=size
        for i in tuple(range(number1)):
            combi1*=x       # kombináció    size!/((size-number1)! * number1!)
            x-=1
        for i in tuple(range(number1)):
            combi1/=(i+1)   
        x=size-number2
        for i in tuple(range(number1-number2)):
            combi2*=x       # kombináció 2.    (size-number2)!/((size-(number1-number2))! * (number1-number2)!)
            x-=1
        for i in tuple(range(number1-number2)):
            combi2/=(i+1)        
        return combi1/combi2    # ods  
   
          

    #******************************************************************************
   
    def randomTip(self):
        ''' véletlen számok generálása  (1 és self.szamtart  között)
    	nem lehet  ismétlődés !!!
    	self.maxszam -->  hány darab számot kell választani
     	self.szamtart-->  tartomány vége, legnagyobb szám (pl. 5-ös lottó esetén 90)  '''
        
        number=0   # egy új szám tárolására
        isIt=True   # volt-e már a szám 
        self.szamok.clear()
        while len(self.szamok)<self.maxszam:        # lista feltöltése
            isIt=True
            while isIt:   # egy új véletlen szám keresése, ami még nem volt
                number=random.randint(1,self.szamtart)   #  1 és "self.szamtart" közötti véletlen szám
                isIt=number in self.szamok    # volt-e már ez a szám ?
            self.szamok.append(number)  # meg van az új szám -> bele a listába
        self.text1szamok.text=str(self.szamok)[1:-1]


    #******************************************************************************
   
    def combinationTip(self):
        ''' Hibapontos tippek előállítása   '''
        
        if self.szamtart==80:
            ods=self.comb2(80,20,self.minszam)
            self.text2tippek.text= "80 számból -> 20 szám húzva \n-> " + str(self.minszam) + " szám eltalálva \n"
        else:
            ods=self.comb(self.szamtart,self.minszam)
            self.text2tippek.text= str(self.szamtart) + " számból -> "+ str(self.minszam) + " szám \n"
        self.text2tippek.text += " esély a telitalálatra 1db tippel -> \n  1 : " + str(ods) 
        show_mypopup()
        tmess=self.tippgen(self.minszam,self.szamok,self.tippek)
        self.text2tippek.text += "\n \n" + tmess + "\n Kombinációk (tippek) száma: " + str(len(self.tippek))
        if self.hiba==0:
            for i in self.tippek:
                self.text2tippek.text += "\n " + str(i)[1:-1]  
        else:
            self.faultcombTip(self.hiba,self.tippek,self.hibatippek)    
            self.text2tippek.text += "\n  Hibapontos komb. száma: " + str(len(self.hibatippek))    
            for i in self.hibatippek:
                self.text2tippek.text += "\n " + str(i)[1:-1]
                   
            
    #******************************************************************************
   
    def tippgen(self,tip,lnumbers,ltips):
        ''' szám kombinációkat állít elő a megadott számokból
    	tip -->  hány darab szám kell egy kombinációhoz
     	lnumbers -->  a számok, amelyek közül választani lehet, lista !
        ltips --> listák listája  a kombinációk (tippek) tárolására  '''
        
        ltip1=[]   # lista  egy tipp tárolására    
        number=0   # egy új szám tárolására    
        index=[]   # ?
        size=0  # lnumbers lista mérete
        generate=True   # ?
        size=len(lnumbers)
        ltips.clear()
        if size<tip:
            return " valami gond van a paraméterekkel !! 'tip' túl nagy !!"
        else:
            index=list(range(tip))
            cycl=1
            while generate:
                ltip1.clear()
                #print(" index:  ",index)
                for i in tuple(range(tip)):  # egy új kombináció előállítása
                    number=lnumbers[index[i]]
                    ltip1.append(number)
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
           
            return "tippek generálva"    
       
       
    #******************************************************************************
    # 	 
    #******************************************************************************
    def faultcombTip(self,fault,ltips,lfaulttips):   
        ''' hibapontos kombinációt állít elő 
        a megadott kombinációból ("megszűri" a kombinációt)    
    	fault -->  hiba szám (mennyi eltérés lehet)
     	ltips -->  bemenő  számsorok (tippek), amelyek közül választani kell (szűrni kell)
        lfaulttips --> listák listája, az eredmény (megszűrt) tippsorok   '''
        
        lfiltered=[]   # egy tipsor tárolása, munka lista
        diff=0    #  eltérő számok  száma
        tipsize=0   #  tippsorok száma
        filteredsize=0  #  szűrt tippsorok száma
        OK=True
        lfaulttips.clear()
        tipsize=len(ltips)

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

def show_mypopup():
    content1 = Button(text='Számolok !!',font_size=40)
    mypopup = Popup(title="Figyelem",content=content1, auto_dismiss=False,
              size_hint= (1/2, 1/2))
    content1.bind(on_press=mypopup.dismiss)         
    mypopup.open()


class SzerencseApp(App):
    def build(self):   
        return LuckyWidget()


if __name__ == '__main__':
    szer= SzerencseApp()
    szer.run()

