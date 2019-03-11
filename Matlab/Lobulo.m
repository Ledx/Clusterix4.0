function a=Lobulo(thetha)
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
    
    if (thetha>=0 && thetha<=((60*pi)/180) || (thetha>=(300*pi/180) && thetha<((2*pi))))
        a=1;
    else
        a=0;
    end
    
end