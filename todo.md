Software f�r Heimautomatisierung: 
	- Node meldet sich mit standard-ID (1 oder 0?) alle x-sekunden (10?) und wartet auf ein ACK des Masters
		? gr��e der Listen
		? liste mit enthaltenen sensoren
		? liste mit enthaltenen aktoren
		? direktanalysemodule? schritt-erkennung, sofortsenden?
	- Master sendet ACK um die Aufnahme abzuschlie�en
		? weist neue ID zu
		? weist updateintervall zu
		? aktivierte Sensoren 
		? soll ein ACK bei jedem Senden abgewartet werden?
		? soll der Node immer aktiv sein und Nachrichten weiterleiten?
		? sende aktuelle uhrzeit mit
		? speichern der config im eprom? --> nein: speicher �berschreiben
	- Node: 
		? speichern der Config im Eprom?
		
		
Je sensoranmeldung eine ID
Aktiviert. Aufl�sung wert. Aufl�sung Zeit. Min max standard. Name string.
Setze: freq. Aktiv.
Nutze RAM zum Zwischenspeicher
Node meldet sich. Master fragt. Node sendet. Master ACK.
Aktormodell anders

Library: 
	- Schreiben des Eproms
	- RTC
	- Schlafen
	- Funkchip

Weitere sensoren:
	- Batteriespannung
	- Paketzahlzahl
	- Zeit / unix
	
	- Einheit string

Node Standort protokollversion
Aktor
