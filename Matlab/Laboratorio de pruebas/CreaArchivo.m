function archivo=CreaArchivo(identificador,tipo) 
%Funcion que realiza la creacion de un archivo de salida

    tipos_archivos = ["-Parametros_iniciales.txt" "-Histograma.txt" "-Clusters.txt" "-Tiempo.txt"];
    archivo=fopen(strcat(identificador,tipos_archivos(tipo)),'w'); 
    
end