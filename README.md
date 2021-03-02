# MapReduce-System
<div align="center">
    <br />
    <img src="./documentation/Logo.png" alt="MapReduce_System_Logo" width="350"/>
</div>


## Grill Alexander NVS Projekt 2021
Die Lage in Österreich bezgl. COVID-19 ist auch Jahr 2021, im zweiten Schulsemster ernst. Ein Lockdown nach dem anderem, Festlegungen von Maßnahmen und Ausgangsbeschränkungen,
die Anzahl der aktiven Fälle steigt, aus diesem Grund bleibt der Bundesregierung nichts anderes über und muss so agieren um die Bevölerunge zu schützen.
Deshalb wurden Schulerunterrichtstunden online gehalten bis Ende Jännder, ab diesem Zeitpunkt wurde eine Art Schichbetrieb in den Schulen gehalten.
Foglendessen hat Herr Professor Kolousek beschlossen, dass die Schüler auch im 2 Semester ein NVS Projekt erarbeiten müssen, weil es zu aufwändig wäre eine Praktische Arbeit und Theorietest zu machen. Diese Repo umfasst die Umsetzung des Projekt, als auch die dazugehörige Dokumentation und nötigen Grundlagen.


Das Projekt deckt folgende Themen ab: Grundlagen und Basiskonzepte, Nachrichtenübertragung, Netzarchitektur, Internetprotokoll, Transportprotokolle, Prozesse und Threads, Synchronisation und parallele Programmierung, Kommunikation, Serverprogrammierung, verteilte Systeme, TCP/IP Programmierung wie im Beispiel umgesetzt

### MapReduce-System
In diesem Projekt geht es darum, ein einfaches MapReduce‐System samt Beispielanwendung inkl. Aufarbeitung des Themas zu erstellen. Dabei sollen Daten die in unstrukturierter
Form abgespeichert sind, zusammengefasst und geordnet werden, sodass diese dann daraufhin für weitere Verarbeitungen und Analysen der Daten durchgeführt werden kann. Die Daten werden am Beginn aufgesplittet und in Arbeitspackete aufgeteilt. Diese werden folgedessen, von parallel laufenden Prozesse verarbeitet und das Ergebniss dann einen weiteren Knoten 
zugesendet. Dies erfolgt so lange bis alle Daten, die am Beginn auf mehreren Knoten aufgeteilt wurde, am Master Server zusammengefasst abgespeichert werden können.
