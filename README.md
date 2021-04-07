# MapReduce-System
<div align="center">
    <br />
    <img src="./documentation/Logo.png" alt="MapReduce_System_Logo" width="350"/>
</div>
<img alt="License" src="https://img.shields.io/github/license/alexandergrill/grill_project_2/">

## Grill Alexander NVS Projekt 2021
Die Lage in Österreich bezgl. COVID-19 ist auch im Jahr 2021, im zweiten Schulsemster ernst. Ein Lockdown nach dem anderem, Festlegungen von Maßnahmen und Ausgangsbeschränkungen,
die Anzahl der aktiven Fälle steigt, daher muss die Bundesregierung mit solchen stengen Maßnahmen agieren, um die Bevölerunge zu schützen.
So wurden Schulerunterrichtstunden bis Ende Jänner online gehaltet, anschließend erfolgte Schichbetrieb in den Schulen.
Foglendessen hat Herr Professor Kolousek beschlossen, dass die Schüler auch im 2.Semester ein NVS Projekt erarbeiten müssen, weil es zu aufwändig wäre, eine "Praktische Arbeit" und Theorietest zu machen. Diese Repo umfasst die Umsetzung des Projekts, als auch die dazugehörige Dokumentation und nötigen Grundlagen.

Das Projekt deckt folgende Themen ab: Grundlagen und Basiskonzepte, Nachrichtenübertragung, Netzarchitektur, Internetprotokoll, Transportprotokolle, Prozesse und Threads, Synchronisation und parallele Programmierung, Kommunikation, Serverprogrammierung, verteilte Systeme, TCP/IP Programmierung wie im Beispiel umgesetzt

### MapReduce-System
In diesem Projekt geht es darum, ein einfaches MapReduce‐System samt Beispielanwendung inkl. Aufarbeitung des Themas zu erstellen. Dabei sollen Daten die in unstrukturierter
Form abgespeichert sind, zusammengefasst und geordnet werden, sodass diese dann daraufhin für weitere Verarbeitungen und Analysen der Daten durchgeführt werden kann. Die Daten werden am Beginn aufgesplittet und in Arbeitspackete aufgeteilt. Diese werden folgedessen, von parallel laufenden Prozesse verarbeitet und das Ergebnis einen weiteren Knoten 
zugesendet. Dies erfolgt so lange bis alle Daten, die am Beginn auf mehreren Knoten aufgeteilt wurden, am Master Server zusammengefasst abgespeichert werden können.

### Client

#### Demo

#### Bedienung

```
./client -h
```
Dadurch erhält man alle Information bezgl. der Kommandozeilenparameter
```
./client -p 1113
```
Startet den Client mit der IP Adresse 127.0.0.1 und versucht sich auf Port 1113 zum SlaveServer zu verbinden und sendet den SlaveServer die Daten(Result der Map Funktion)
Der Port ist beim Aufruf des Programms zwingend notwendig. Es werden, ohne Aufruf von -w "ANZAHL", 50 000 unterschiedliche strings in die Datei clientfile.txt geschreiben und
folgedessen in der Map Funktion komprimiert.
```
./client -p 1113 -i 192.168.8.1
```
Startet den Client mit der IP Adresse 192.168.8.1 und versucht sich auf Port 1113 zum SlaveServer zu verbinden und sendet den SlaveServer die Daten(Result der Map Funktion)
```
./client -p 1113 -w 100000
```
Startet den Client mit der IP Adresse 127.0.0.1 und versucht sich auf Port 1113 zum SlaveServer zu verbinden und sendet den SlaveServer die Daten(Result der Map Funktion)
Dabei werden 100 000 unterschiedliche Strings in eine Datei geschrieben, die in der Map Funktion komprimiert werden. 
```
./client -p 1113 -f "../Desktop/file.txt"
```
Startet den Client mit der IP Adresse 127.0.0.1 und versucht sich auf Port 1113 zum SlaveServer zu verbinden und sendet den SlaveServer die Daten(Result der Map Funktion)
In diesem Fall werden die random generierten strings in die Datei file.txt abgespeichert.

### Slave Server

#### Demo

#### Bedienung

```
./slaveserver -h
```
Dadurch erhält man alle Information bezgl. der Kommandozeilenparameter
```
./slaveserver -s 1113 -p 1116
```
Startet den SlaveServer mit der IP Adresse 127.0.0.1 und versucht sich auf Port 1116 zum MasterServer zu verbinden und sendet den MasterServer die Daten(Result der Shuffle Funktion). Der SlaveServer hört auf Port 1113 ab und baut eine Verbindung bei einem Request eines Clients auf. Der SlaveServer empfängt alle Daten der Clients.
Die Ports sind beim Aufruf des Programms zwingend notwendig. 
```
./slaveserver -s 1113 -p 1116 -i 192.168.8.1
```
Startet den SlaveServer mit der IP Adresse 192.168.8.1 und versucht sich auf Port 1116 zum MasterServer zu verbinden und sendet den MasterServer die Daten(Result der Shuffle Funktion)
```
./slaveserver -s 1113 -p 1116 -c 6
```
Der SlaveServer 
