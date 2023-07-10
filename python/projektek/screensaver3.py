#!/usr/bin/env python3
# -*- coding:Utf-8 -*-
# vonalak 0.1
# vonalak mozognak

from tkinter import *
from random import randrange

#=============================================================================#
class Pont:
	def __init__(self,xk,yk):
		self.x=xk
		self.y=yk

#=============================================================================#
class Mozgvonalak:
	# mozgás
	
	def __init__(self,szel=400,mag=400,pont=3):
		self.xmeret=szel			# vászon méret
		self.ymeret=mag
		self.pontdb=pont		# végpontok száma
		self.pontok=[]			# végpontokat tartalmazó lista
		self.vektorok=[]		# irány vektorokat tartalmazó lista
		self.vonalak=[]			# felrajzolt vonalakat tartalmazó lista
		self.flag =0			#elmozdulás jelző,  0 ---> nincs
		i=0
		while (i<pont):
			x1=randrange(10,szel-10)		# kezdő koordináták
			y1=randrange(10,mag-10)
			p1=Pont(x1,y1)
			self.pontok.append(p1)
			vx1=randrange(1,31)		# irány vektorok
			vy1=randrange(1,31)
			p2=Pont(vx1-15,vy1-15)	# legyen pozitív és negatív is
			self.vektorok.append(p2)
			i+=1
						
		self.abl1 = Tk()
		self.abl1.title("Screensaver")
		self.vaszon = Canvas(self.abl1,bg='dark grey',height=mag,width=szel)
		self.vaszon.pack(side=LEFT)			# a vászon létrehozása
		Button(self.abl1,text='Kilép',command=self.abl1.quit).pack(side=BOTTOM)
		Button(self.abl1,text='Indít',command=self.start_move).pack()
		Button(self.abl1,text='Leállít',command=self.stop_move).pack()
		# vonalak felrajzolása:
		i=0
		while(i<pont):
			j=(i+1)		#a pontok listájából az  i.-dik --> az egyik végpont
			if (j==pont):			# a j.-edik a másik végpont lesz
				j=0
			von = self.vaszon.create_line(self.pontok[i].x,self.pontok[i].y,self.pontok[j].x,self.pontok[j].y,width=2,fill='red')
			self.vonalak.append(von)
			i+=1
		# az eseményfogadó indítása (főciklus) :
		self.abl1.mainloop()
		
#-----------------------------------------------------------------------------#		
	def move(self):
		"a labda elmozdulása"
		
		i=0
		while(i<self.pontdb):
			self.pontok[i].x += self.vektorok[i].x 
			self.pontok[i].y += self.vektorok[i].y 
			
			if self.pontok[i].x >=self.xmeret:  # ha kimenne jobbra
				self.pontok[i].x=self.xmeret-1
				self.vektorok[i].x*=-1		# vizszintes irányvektor balra
			if self.pontok[i].y >=self.ymeret:	# ha kimenne lentre
				self.pontok[i].y=self.ymeret-1
				self.vektorok[i].y*=-1	# függőleges irányvektor felfelé
			if self.pontok[i].x <=0:		# ha kimenne balra
				self.pontok[i].x=1
				self.vektorok[i].x*=-1
			if self.pontok[i].y <=0:	# ha kimenne felfelé
				self.pontok[i].y=1
				self.vektorok[i].y*=-1
			i+=1
		# régi vonalak törlése:
		
		for i in self.vonalak:	
			self.vaszon.delete(i)
			self.vonalak=[]
		# új vonalak rajzolása:
		i=0
		while(i<self.pontdb):
			j=i+1
			if (j==self.pontdb):
				j=0
			von = self.vaszon.create_line(self.pontok[i].x,self.pontok[i].y,self.pontok[j].x,self.pontok[j].y,width=2,fill='red')
			self.vonalak.append(von)
			i+=1		
				
		if self.flag >0:
				self.abl1.after(30, self.move)
				# 30 millisec után ciklus
		
#-----------------------------------------------------------------------------#		
	def stop_move(self):
		"leáll"		
		self.flag =0
		
#-----------------------------------------------------------------------------#	
	def start_move(self):
		"mozgás elindítása"
		self.flag += 1
		# flag beállítása az indításhoz :
		if self.flag ==1:
			self.move()
	
	
#========== Főprogram ========================================================#

if __name__=='__main__':
	
	mozg1=Mozgvonalak(pont=6)
	


     # *** itt a vége! *** 






