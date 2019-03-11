function y=Clustered(x1,x2,R_COLISION)
%Funcion que determina si dos robots se encuentran en un mismo cluster

    if norm(x1-x2)<=R_COLISION
        y=1;
    else
        y=0;
    end

end