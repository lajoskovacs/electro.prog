# R-L    R-C  L-C szűrők
# induktív és kapacitív reaktancia, 
# eredő impedancia és 
# feszültségátvitel számítások
# 2022.03.01 - 2022.04.08.  KL

# import os
# os.environ['KIVY_NO_CONSOLELOG']='1'

from kivy.config import Config 
Config.set('graphics','width','400')
Config.set('graphics','height','600')
# Config.set('graphics','resizable','0')

from kivy.app import App
from kivy.uix.button import  Button
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty
from kivy.lang import Builder
from kivy.graphics import Color, Ellipse, Line, Rectangle, Point, GraphicException
from math import pi
from math import sqrt
from math import atan


Builder.load_string('''

<XcalcWidget>:
	drlab1:  drlab1
    lab1rlc: lab1rlc
    lab2rlc: lab2rlc    
    txt1rlc: txt1rlc
    txt2rlc: txt2rlc    
    txt3f:   txt3f
    txt4xl:  txt4xl
    txt5xc:  txt5xc
    txt6zbe: txt6zbe
    txt7atv: txt7atv
    txt8fi:  txt8fi
    txt9fh:  txt9fh 
    but2sel: but2sel
    
    
	Button:				
		id: drlab1
		text: ''
		pos_hint: {'top':1}
        size_hint: 0.3, 0.3
        on_size: root.circdraw()
        
    GridLayout:
		cols: 2
        spacing: 2, 2 
        pos_hint: {'right':1,'top':1}
        size_hint: 0.6, 0.3
        
        Label:
            id: lab1rlc
            text: 'R (k\u03A9)'
            size_hint: .4, .25 

        TextInput:
            id: txt1rlc
            size_hint: .6, .25 
            background_color: 1, 0, 1, 1         
            color: 0, 0, 1, 1
 
        Label:
            id: lab2rlc
            text: 'L (mH)'
            size_hint: .4, .25 

        TextInput:
            id: txt2rlc
            size_hint: .6, .25 
            background_color: 1, 0, 1, 1         
            color: 0, 0, 1, 1 
        
        Label:
            text: 'f (Hz)'
            size_hint: .4, .25 
            
        TextInput:
            id: txt3f
            size_hint: .6, .25
            background_color: 1, 0, 1, 1         
            color: 0, 0, 1, 1         
        
        Button:
            id: but2sel
            text: 'RL'
            size_hint: .4, .25
            on_press: root.circsel()
            on_text: root.circdraw()
            
        Button:
            text: 'Számolás'
            size_hint: .6, .25  
            on_press: root.circcalc()        
        
     
    GridLayout:   
        cols: 2
        spacing: 2, 2 
		pos_hint: {'x':0.1,'y':0.2}        
        size_hint: 0.8, 0.4

        Label:
            text: 'XL (k\u03A9)'
            size_hint: .1, .15  
            
        Label:
            id: txt4xl
            size_hint: .8, .15 
            
        Label:
            text: 'Xc (k\u03A9)'
            size_hint: .1, .15  
            
        Label:
            id: txt5xc
            size_hint: .8, .15   
                                    
        Label:
            text: 'Zbe (k\u03A9)'
            size_hint: .1, .15  
            
        Label:
            id: txt6zbe
            size_hint: .8, .15 
            
        Label:
            text: 'Uki/Ube'
            size_hint: .1, .15  
            
        Label:
            id: txt7atv
            size_hint: .8, .15 
                                     
        Label:
            text: '\u03C6 (ki-be)'
            size_hint: .1, .15  
            
        Label:
            id: txt8fi
            size_hint: .8, .15  
                        
        Label:
            text: 'fh (Hz)'
            size_hint: .1, .15
            
        Label:
            id: txt9fh
            size_hint: .8, .15 
                                          
                                                           
''')



class XcalcWidget(FloatLayout):
	
	
    def __init__(self):
        super().__init__()
        self.circtyp='RL'
        self.circuits=['RL','LR','RC','CR','LC','CL']


    def circsel(self):
        """ áramkör választás
        R-L  L-R   R-C  C-R  L-C  C-L	"""      
        i=self.circuits.index(self.circtyp)
        self.circtyp=self.circuits[(i+1)%len(self.circuits)]  # áramkör léptetése
        self.but2sel.text= self.circtyp     # áramkör kiírása a nyomógombra
        if self.circtyp=='RL':      # beviteli szövegmezők felíratának módosítása
            self.lab1rlc.text='R (k\u03A9)'
            self.lab2rlc.text='L (mH)'
        elif self.circtyp=='LR':
            self.lab2rlc.text='R (k\u03A9)'
            self.lab1rlc.text='L (mH)'
        elif self.circtyp=='RC':
            self.lab1rlc.text='R (k\u03A9)'
            self.lab2rlc.text='C (nF)'
        elif self.circtyp=='CR':
            self.lab2rlc.text='R (k\u03A9)'
            self.lab1rlc.text='C (nF)'
        elif self.circtyp=='LC':
            self.lab1rlc.text='L (mH)'
            self.lab2rlc.text='C (nF)'
        elif self.circtyp=='CL':
            self.lab2rlc.text='L (mH)'
            self.lab1rlc.text='C (nF)'  
                              
        self.txt1rlc.text = ''   # régi értékek, számítások törlése
        self.txt2rlc.text = ''   
        self.txt3f.text = ''
        self.txt4xl.text = ''     
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
 
        
    def circcalc(self):
        ''' áramköri számítások '''
                
        if self.circtyp=='RL':      # számítások meghívása
            self.calc1rl()
        elif self.circtyp=='LR':
            self.calc2lr()
        elif self.circtyp=='RC':
            self.calc3rc()
        elif self.circtyp=='CR':
            self.calc4cr()
        elif self.circtyp=='LC':
            self.calc5lc()
        elif self.circtyp=='CL':
            self.calc6cl()  
       
            
    def calc1rl(self):
        ''' számítás, ha a felső ág (felső szövegmező) - ellenállás,  
        kimeneten (alsó szövegmező) - induktív reaktancia,	
        'L' értéke 'mH'-ben megadva !!		'''
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            L=float(self.txt2rlc.text)   # 'L' beolvasása szövegmezőből
            if L<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xl=2*pi*f*L/1000000         #  XL   kOhm-ban !!
            self.txt4xl.text = str(xl)   # 'XL' kiírása szövegmezőbe
        try:
            R=float(self.txt1rlc.text)   # 'R' beolvasása szövegmezőből
            if R<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !
        if ok:        
            Zbe=sqrt(R**2+xl**2)        # Zbe  számítása
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=xl/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            fi=180*atan(R/xl)/pi        # fázistolás
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000*R/(2*pi*L)       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
                       

           
    def calc2lr(self):
        ''' számítás, ha a felső ág (felső szövegmező) - induktív reaktancia, 
        kimeneten (alsó szövegmező) - ellenállás		
        'L' értéke 'mH'-ben megadva !!  '''		
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            L=float(self.txt1rlc.text)   # 'L' beolvasása szövegmezőből
            if L<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xl=2*pi*f*L/1000000         #  XL   kOhm-ban !!
            self.txt4xl.text = str(xl)   # 'XL' kiírása szövegmezőbe
        try:
            R=float(self.txt2rlc.text)   # 'R' beolvasása szövegmezőből
            if R<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !             
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        if ok:        
            Zbe=sqrt(R**2+xl**2)        # Zbe  számítása
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=R/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            fi=-180*atan(xl/R)/pi        # fázistolás
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000*R/(2*pi*L)       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
            

    def calc3rc(self):
        '''számítás, ha a felső ág (felső szövegmező) - ellenállás,  
        kimeneten (alsó szövegmező) - kapacitív reaktancia,	
        'C' értéke 'nF'-ban megadva !!		'''
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            C=float(self.txt2rlc.text)   # 'C' beolvasása szövegmezőből
            if C<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xc=1000000/(2*pi*f*C)         #  XC   kOhm-ban !!
            self.txt5xc.text = str(xc)   # 'XC' kiírása szövegmezőbe
        try:
            R=float(self.txt1rlc.text)   # 'R' beolvasása szövegmezőből
            if R<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !             
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !
        if ok:        
            Zbe=sqrt(R**2+xc**2)        # Zbe  számítása
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=xc/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            fi=-180*atan(R/xc)/pi        # fázistolás
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000/(2*pi*R*C)       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
                       

    def calc4cr(self):
        ''' számítás, ha a felső ág (felső szövegmező) - kapacitív reaktancia,  
        kimeneten (alsó szövegmező) - ellenállás,	
        'C' értéke 'nF'-ban megadva !!		'''
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            C=float(self.txt1rlc.text)   # 'C' beolvasása szövegmezőből
            if C<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xc=1000000/(2*pi*f*C)         #  XC   kOhm-ban !!
            self.txt5xc.text = str(xc)   # 'XC' kiírása szövegmezőbe
        try:
            R=float(self.txt2rlc.text)   # 'R' beolvasása szövegmezőből
            if R<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték ! 
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        if ok:        
            Zbe=sqrt(R**2+xc**2)        # Zbe  számítása
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=R/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            fi=180*atan(xc/R)/pi        # fázistolás
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000/(2*pi*R*C)       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
                       

    def calc5lc(self):
        ''' számítás, ha a felső ág (felső szövegmező) - induktív reaktancia, 
        kimeneten (alsó szövegmező) - kapacitív reaktancia		
        'L' értéke 'mH'-ben,  'C' értéke 'nF'-ban megadva !!	 '''	
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            L=float(self.txt1rlc.text)   # 'L' beolvasása szövegmezőből
            if L<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !
        try:
            C=float(self.txt2rlc.text)   # 'C' beolvasása szövegmezőből
            if C<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !                 
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !               
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xl=2*pi*f*L/1000000         #  XL   kOhm-ban !!
            self.txt4xl.text = str(xl)   # 'XL' kiírása szövegmezőbe
            xc=1000000/(2*pi*f*C)         #  XC   kOhm-ban !!
            self.txt5xc.text = str(xc)   # 'XC' kiírása szövegmezőbe
            if xl>xc:
                Zbe=xl-xc                 # Zbe  számítása
            else:
                Zbe=xc-xl
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=xc/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            if xl>xc:
                fi=180                 # fi  számítása
            else:
                fi=0            
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000/(2*pi*sqrt(L*C))       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
            

    def calc6cl(self):
        ''' számítás, ha a felső ág (felső szövegmező) - kapacitív reaktancia, 
        kimeneten (alsó szövegmező) - induktív reaktancia		
        'L' értéke 'mH'-ben,  'C' értéke 'nF'-ban megadva !!		'''
        ok=True 
        self.txt4xl.text = ''
        self.txt5xc.text = ''
        self.txt6zbe.text = ''
        self.txt7atv.text = ''
        self.txt8fi.text = ''
        self.txt9fh.text = ''
        
        try:
            L=float(self.txt2rlc.text)   # 'L' beolvasása szövegmezőből
            if L<=0:
                ok=False
                self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt2rlc.text=self.txt2rlc.text + ' ?'   # nem jó érték !
        try:
            C=float(self.txt1rlc.text)   # 'C' beolvasása szövegmezőből
            if C<=0:
                ok=False
                self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt1rlc.text=self.txt1rlc.text + ' ?'   # nem jó érték !                 
        try:
            f=float(self.txt3f.text)   # 'f' beolvasása szövegmezőből
            if f<=0:
                ok=False
                self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !            
        except:
            ok=False
            self.txt3f.text=self.txt3f.text + ' ?'   # nem jó érték !
        if ok:
            xl=2*pi*f*L/1000000         #  XL   kOhm-ban !!
            self.txt4xl.text = str(xl)   # 'XL' kiírása szövegmezőbe
            xc=1000000/(2*pi*f*C)         #  XC   kOhm-ban !!
            self.txt5xc.text = str(xc)   # 'XC' kiírása szövegmezőbe
            if xl>xc:
                Zbe=xl-xc                 # Zbe  számítása
            else:
                Zbe=xc-xl
            self.txt6zbe.text= str(Zbe)   # 'Zbe' kiírása szövegmezőbe
            atv=xl/Zbe                   # Uki/Ube  számítása
            self.txt7atv.text= str(atv)   # 'Uki/Ube' kiírása szövegmezőbe
            if xl>xc:
                fi=0                 # fi  számítása
            else:
                fi=180            
            self.txt8fi.text= str(fi)   # 'fi' kiírása szövegmezőbe
            fh=1000000/(2*pi*sqrt(L*C))       # határfrekvencia számítása
            self.txt9fh.text= str(fh)   # 'fh' kiírása szövegmezőbe            
         
         
    def circdraw(self):
        ''' áramkör kirajzoló függvény   '''
        ymax=self.size[1]
        xe=self.drlab1.size[0]/50
        ye=self.drlab1.size[1]/50
        ctyp=self.circtyp
        if xe>ye:
            xe=ye
        else:
            ye=xe
        self.drlab1.canvas.clear()    
        with self.drlab1.canvas:
             #Color(1, 1, 1)
            #Rectangle(pos=[0,0],size=self.drlab1.size)
            Color(1, 0, 0)           
            Line(points=[10*xe,ymax-34*ye,40*xe,ymax-34*ye],width=2)    # alsó vonal
            # felső alkatrész
            if ctyp=='RL' or ctyp=='RC':                # ellenállás felül
                Line(points=[10*xe,ymax-10*ye,16*xe,ymax-10*ye],width=2)  # bal kivezetés
                Line(rectangle=(16*xe,ymax-12*ye,12*xe,4*ye),width=2)       # R test
                Line(points=[28*xe,ymax-10*ye,40*xe,ymax-10*ye],width=2)    # jobb kivezetés
            elif ctyp=='CR' or ctyp=='CL':              # kondi felül
                Line(points=[10*xe,ymax-10*ye,20*xe,ymax-10*ye],width=2)  # bal kivezetés
                Line(points=[20*xe,ymax-7*ye,20*xe,ymax-13*ye],width=2)       # C test
                Line(points=[23*xe,ymax-7*ye,23*xe,ymax-13*ye],width=2)
                Line(points=[23*xe,ymax-10*ye,40*xe,ymax-10*ye],width=2)    # jobb kivezetés
            elif ctyp=='LR' or ctyp=='LC':               # tekercs felül
                Line(points=[10*xe,ymax-10*ye,14*xe,ymax-10*ye],width=2)  # bal kivezetés
                Line(circle=(16*xe,ymax-10*ye,2*xe,90,-90),width=2)       # L test
                Line(circle=(20*xe,ymax-10*ye,2*xe,90,-90),width=2)
                Line(circle=(24*xe,ymax-10*ye,2*xe,90,-90),width=2)
                Line(circle=(28*xe,ymax-10*ye,2*xe,90,-90),width=2)
                Line(points=[30*xe,ymax-10*ye,40*xe,ymax-10*ye],width=2)    # jobb kivezetés                
             # kimeneti alkatrész
            if ctyp=='LR' or ctyp=='CR':                # ellenállás a kimeneten
                Line(points=[34*xe,ymax-10*ye,34*xe,ymax-16*ye],width=2)  # felső kivezetés  
                Line(rectangle=(32*xe,ymax-28*ye,4*xe,12*ye),width=2)       # R test
                Line(points=[34*xe,ymax-28*ye,34*xe,ymax-34*ye],width=2)  # alsó kivezetés 
            elif ctyp=='RC' or ctyp=='LC':              # kondi a kimeneten
                Line(points=[34*xe,ymax-10*ye,34*xe,ymax-20*ye],width=2)  # felső kivezetés
                Line(points=[31*xe,ymax-20*ye,37*xe,ymax-20*ye],width=2)       # C test
                Line(points=[31*xe,ymax-23*ye,37*xe,ymax-23*ye],width=2)
                Line(points=[34*xe,ymax-23*ye,34*xe,ymax-34*ye],width=2)    # alsó kivezetés
            elif ctyp=='RL' or ctyp=='CL':               # tekercs a kimeneten
                Line(points=[34*xe,ymax-10*ye,34*xe,ymax-14*ye],width=2)  # felső kivezetés
                Line(circle=(34*xe,ymax-16*ye,2*xe,0,180),width=2)       # L test
                Line(circle=(34*xe,ymax-20*ye,2*xe,0,180),width=2)
                Line(circle=(34*xe,ymax-24*ye,2*xe,0,180),width=2)
                Line(circle=(34*xe,ymax-28*ye,2*xe,0,180),width=2)
                Line(points=[34*xe,ymax-30*ye,34*xe,ymax-34*ye],width=2)    # alsó kivezetés 



class XcalcApp(App):
    def build(self):       
        return XcalcWidget()


if __name__ == '__main__':
    xc= XcalcApp()
    xc.run()

