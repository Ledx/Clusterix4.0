function [x,y]=GeneradorCircunferencia(x1,x2)
%Funcion que genera una circunferencia con centro en el centroide de un
%cluster
    c = [(x1(1)+x2(1))/2,(x1(2)+x2(2))/2];
   
     t=0:pi/30:2*pi; 
     x=10*cos(t)+c(1);
     y=10*sin(t)+c(2); 
    

end