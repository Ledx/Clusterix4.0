function [x,y]=DIB_Circunferencia(c,r)
%Funcion que dibuja una circunferencia con centro c y radio r
   
     t=0:pi/30:2*pi; 
     x=r*cos(t)+c(1);
     y=r*sin(t)+c(2); 
    

end