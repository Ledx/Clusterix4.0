function thetha = BarridoFrecuencia(robot_referencia,robots,DELTA_ROTACION,ALFA_POTENCIAL,N,SIGMA_ROTACION,MEDIA_RUIDO,SIGMA_POTENCIAL)
%Funcion que realiza la rotacion y el calculo de un angulo en una
%frecuencia
    
    %robots(robot_referencia).thethaIncerteza=robots(robot_referencia).thetha;
    compx=zeros(N,1);
    compy=zeros(N,1);
    potencialDireccional=zeros(N,1);
    for frecuencia=1:N
        giro=0;
        
        while giro<=(2*pi)
            k = 1;
            eta = [];
            phi = [];

            for robot_lejano=1:N
                alfa = 0;
                if robot_referencia ~= robot_lejano && robots(robot_lejano).clase >= frecuencia

                    alfa= atan2(robots(robot_lejano).posicionRect.y-robots(robot_referencia).posicionRect.y,robots(robot_lejano).posicionRect.x-robots(robot_referencia).posicionRect.x);
                    if alfa < 0
                        alfa = alfa + (2*pi);
                    end
                    eta(k)= LobuloCos(abs(alfa-robots(robot_referencia).thetha));
                    phi(k) = Potencial([robots(robot_referencia).posicionRect.x,robots(robot_referencia).posicionRect.y],[robots(robot_lejano).posicionRect.x,robots(robot_lejano).posicionRect.y],robots(robot_lejano).constante_potencial,ALFA_POTENCIAL,MEDIA_RUIDO,SIGMA_POTENCIAL);
                    k=k+1;
                end
            end

            potencialDireccional(frecuencia) = dot(eta,phi);
            compx(frecuencia) = ( potencialDireccional(frecuencia)*cos(robots(robot_referencia).thetha) )+compx(frecuencia);
            compy(frecuencia) = ( potencialDireccional(frecuencia)*sin(robots(robot_referencia).thetha) )+compy(frecuencia);
            %robots(robot_referencia).thetha = robots(robot_referencia).thetha + DELTA_ROTACION;
            robots(robot_referencia).thetha = robots(robot_referencia).thetha + normrnd(DELTA_ROTACION,SIGMA_ROTACION);
            if robots(robot_referencia).thetha > (2*pi)
                while robots(robot_referencia).thetha > (2*pi)
                    robots(robot_referencia).thetha = robots(robot_referencia).thetha-(2*pi);
                    %robots(robot_referencia).thethaIncerteza = robots(robot_referencia).thethaIncerteza-(2*pi);
                end
            end
            giro = giro + DELTA_ROTACION;

        end
    end
    
    thetha = atan2(compy,compx);
%     if thetha < 0
%         while thetha < 0
%             thetha=thetha+(2*pi);
%         end
%     end
%     if thetha > (2*pi)
%         while thetha > (2*pi)
%             thetha=thetha-(2*pi);
%         end
%     end
    
end

