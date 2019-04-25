function robots = AsignadorClase(robots,N)
%Asigna la clase a cada robot de acuerdo al tamaño de cluster al que
%pertenece

    clase = 1;
    colision = 1;
    for h=1:N
        for l=1:N
            if   h ~= l && Clustered([robots(h).posicionRect.x,robots(h).posicionRect.y],[robots(l).posicionRect.x,robots(l).posicionRect.y],20) >0 
                robots(h).vecindad(l) = 1;
                robots(l).vecindad(h) = 1;
                colision = 0;
            end
            if ~colision
                clase = clase + 1;
            end
            colision = 1;
        end
        robots(h).clase = clase;
        clase = 1;
    end
    
	
end