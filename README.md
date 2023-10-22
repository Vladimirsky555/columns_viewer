# columns_viewer
Программа для сравнения таблиц в двух базах данных MariaDB

\columns_viewer\bin\data\database.ini - файл с настройками для базы данных. База данных может быть как локальной, так и из интернета.
[mysql]
dbhost=myhost(localhos или любой другой)
dbport=3306
dbname=information_schema
dbuser=login
dbpass=password

password формируется в программе decoder, например для пароля 1234 будет сформирована строка: f3x9eg
