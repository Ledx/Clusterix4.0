function a=LobuloCos(thetha)
%Funcion que calcula la atenuación con la que un robot percibe a otro

    format long
    
    if thetha<0
        while thetha < 0
            thetha=thetha+(2*pi);
        end
    end
    
    if thetha > (2*pi)
        while thetha > (2*pi)
            thetha=thetha-(2*pi);
        end
    end
    
    if thetha>=0 && thetha<=((70*pi)/180)
        a=cos(thetha)^3;
    else
        a=abs(cos(3*thetha))/3;

    end
    
end