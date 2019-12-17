# Proyecto
ENTRAR CONSOLA DE UN CONTENEDOR
´´´
sudo docker exec -it  test-mysql mysql -uroot -proot
´´´

OBTENER LA DIRECCION IP DE UN CONTENEDOR 
´´´
sudo docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' nombrecontainer
´´´

CREAR UN USUARIO EN ESA BASE DE DATOS , para poder conectarse externamente 
´´´
CREATE USER 'username'@'localhost' IDENTIFIED BY 'password';

GRANT ALL PRIVILEGES ON *.* TO 'username'@'localhost' WITH GRANT OPTION;

CREATE USER 'username'@'%' IDENTIFIED BY 'password';

GRANT ALL PRIVILEGES ON *.* TO 'username'@'%' WITH GRANT OPTION;

FLUSH PRIVILEGES;
´´´

Entrar en el docker de postgres
´´´  
sudo docker exec -it pg-docker psql -h localhost -U postgres -d postgres

\c => selecciona base de datos
\c personal
\dt => muestra las tablas
´´´

Crear tabla personal
´´´
DROP TABLE IF EXISTS personal;

CREATE TABLE personal (
  id SERIAL PRIMARY KEY,
  NAME varchar(255) default NULL,
  INGRESO varchar(255)
);
´´´