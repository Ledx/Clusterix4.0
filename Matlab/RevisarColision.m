function [robots,colision]=RevisarColision(robots,h,l,R_COLISION)
%Revisa si existe colision entre dos robots
    colision = 1;
    if h ~= l && Clustered([robots(h).posicionRect.x,robots(h).posicionRect.y],[robots(l).posicionRect.x,robots(l).posicionRect.y],R_COLISION) > 0 
        robots(l).clustered = 1;
        robots(l).tiempo_estabilidad = 0;
        robots(h).clustered = 1;
        robots(h).vecindad(l) = 1;
        robots(l).vecindad(h) = 1;
        robots(h).tiempo_estabilidad = 0;
        colision = 0;
    end
 
    
	
end