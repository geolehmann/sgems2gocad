sgems2gocad.exe
===============

Konvertierung von SGemS-Grids (als gslib gespeichert) in GOCAD ASCII-Dateien, funktioniert sowohl für 2D- als auch für 3D-Gitter.

Nach dem Start des Programms wird zunächst die Grid-Datei (*.grd) ausgewählt, im zweiten Fenster dann die Datei mit den Daten. Danach wird im selben Verzeichnis eine GOCAD-ASCII-Datei mit einem PointsSet erzeugt, welche die Gitterpunkte von dem Grid mit der modellierten Property enthält.

Beispielformat für die Grid-Datei:
----------------------------------

Grid_daten (206x82x1)  
206  
82  
1  
40  
40  
1.0  
4459680  
5801680  
0  

[Erläuterung: Die 2./3./4. Zeile enthalten die Anzahl der Gitterkästchen, die 5./6./7. Zeile die Größe eines Kästchens, und die letzten drei die Koordinaten des Gitterursprungs.]

Beispielformat für die Daten-Datei:
----------------------------------

Grid_daten (206x82x1)  
1  
Modell_Meter  
6.881919384   
6.87155723572   
6.90880823135   
7.00322914124   
7.14360618591   
7.34677600861   
7.58162927628  
[...]   

