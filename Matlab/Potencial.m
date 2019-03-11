function pot=Potencial(x1,x2,A_POTENCIAL,ALFA_POTENCIAL)
%Funcion que calcula la funcion de potencial de un vector sobre otro
    
    format long
    
    pot = A_POTENCIAL/( norm(x1-x2)^ALFA_POTENCIAL );

end