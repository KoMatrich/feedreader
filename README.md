# feedreader

## Obsah

- [Požadavky](#pozadavky)
- [Spuštění](#spusteni)
- [Užití](#uziti)
- [Popis řešení](#popis-reseni)
    - [Zpravování vstupních argumentů](#zpravovani-vstupnich-argumentu)
    - [Zpravování URL address](#zpravovani-url-address)
    - [Zpracování dat](#zpracovani-dat)
        - [RSS](#rss)
        - [Atom](#atom)
- [Tvůrci](#tvurci)
- [Díky](#diky)
- [Licence](#licence)

## Požadavky

- cmake
- make
- g++
- libxml2
- openssl

## Spuštění
```
$ cd [root dir of project]
$ make
$ cd ./build
$ feedreader <URL>
```

## Užití
```
 feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]

   URL můžou být zabezpečené nebo nezabezpečené

   -f <feedfile>
       specifikuje cestu ke vstupnímu souboru,
       který je použit místo URL.
       Obsahuje seznam URL, které jsou oděleny novým
       řádkem. Řádky začínající '#' jsou ignorovány.

   -c <certfile>
       certfile je certifikát použit pro SSL/TLS
   -C <certaddr>
       certaddr je složka ze které se budou používat certifikáty
   pokud -c nebo -C zadán:
       použijou se defaultní systémové certifikáty

   -T
       shows time informations for each records (if available)
   -a
       shows authors name or email for each record (if available)
   -u
       shows associated URL for each record (if available)
   pokud alespoň jeden z {-T,-a,-u} je zadán:
       vypsané záznamy jsou oděleny dodatečným novým řádkem
```

## Popis řešení

### Zpravování vstupních argumentů

`
Program neprve zpracuje vstupní argumenty a ověří jejich platnost. Dále inicializuje knihovnu Openssl
nezávisle na zabezpečení vstupních URL. Následně zpracuje vstupní soubor pokud byl zadán a uloží všecny
odkazy z daného souboru do dočasné promměnné (není optimální, ale zjednodušuje práci).
`

### Zpravování URL address

`
Program dále inicializuje xmlParser pomocí "XMLParserKeeper", který se stará o následnou dealokaci
při dokončení práce. Podobnou fukci plní konstruktor "WebScraper", který načítá cetrifikáty a
inicializuje kontex pro zabezpečené spojení.
`

`
Následně se zpracovávají jednotlivé odkazy. Nejprve se načte hlavička HTTP odpovodi podle kódu odpovědi
se rozhoduje o dalším postupu při zpracování.
`

|kód|akce|
|-|-|
|301,302,303,307,308|přesměrování|
|200|pokračuje se v načítání obsahu|
|hlavička se nenačetla správně|zkusí se znova načíst url|
|všechy ostatní jsou brány jako špatné|odkaz se přeskočí|

`
Při přesměrování znova pošle HTML request na dané URL. Tento postup se opakuje dokud se program nedostane
na požadovanou koncovou stránku nebo není dosažen limit na přesměrování.
`

`
Při načtení validní hlavičky s kódem 200 se načítá obsah dle hlavičky. Podle "content-lenght" se stanoví
množství charu, které se mají načíst. Při chybějící délce obsahu v HTTP odpovědi se předpokádá,
že obdržená data jsou segmentovaná. Při segmentovaných datech se data načítají po segmentech.
Metadata o délce segmentu jsou odstraněný hned po načtení dat do paměti.
`

### Zpracování dat

`
Následně se data z HTTP se zpracují pomocí libxml a získá se kořen XML stromu. Kořen jméno kořenu určí typ
feedu a zda je validní typ. Následně podle typu feedu se určí způsob zjišťování informací.
Na konec se vytisknou všechny dostupné a požadované informace dle nastavených vstupních argumentů.
`

#### RSS
`
U RSS feedu se zpracovávají pouze XML prvky <channel>. U těchto prvků se předpokládá že budou obsahovat
jako první prvek <title>, který bude obsahovat název kanálu. (Bude vytištěn na stdout ihned po nalezení)
Dále se načítájí jednotlivé prvky <item> z <channel>. Dále vytvořena dočasná proměná pro zajištění správného
pořadí vypsaných atributů z prvku a výpis je zahajen až po zpracování prvku.
`

|xml Element|význam v feedu|
|-|-|
|`<channel>`|Kanál určitého feedu|
|`<title>`|Název kanalu|
|`<item>`|Příspěvek kanalu|
|`<item> -> <title>`|Název příspěvku|
|`<item> -> <autor>`|autor přispěvku|
|`<item> -> <link>`|link příspěvku|
|`<item> -> <pubDate>`|datum příspěvku|

##### struktura RSS:

```
<channel>
  <title>W3Schools Home Page</title>
  <link>https://www.w3schools.com</link>
  <description>Free web building tutorials</description>
  <item>
    <title>RSS Tutorial</title>
    <link>https://www.w3schools.com/xml/xml_rss.asp</link>
    <description>New RSS tutorial on W3Schools</description>
  </item>
  <item>
    ...

    ...
  </item>
</channel>
```

#### Atom

`
U Atom feedu se postupuje podobně jako u RSS feedu s drobnými změnami. Nehledá se prvek <channel>, ale rovnou
název kanalu. Dále se také vytvaří defaultní feed do, kterého se ukládají dostupné defaultní hodnoty.
Následně se procházi všechny prvky <entry>. Načítají se atributy těchto prvků, které se také dočasně ukladají
pro zachovní správného pořadí při tisku.
`

|xml Element|význam v feedu|
|-|-|
|`<title>`|Název kanalu|
|`<entry>`|Příspěvek kanalu|
|`<title>`|Název příspěvku|
|`<autor>`|autor přispěvku|
|`<link>`|má atribut href = link příspěvku|
|`<updated>`|datum příspěvku|

##### struktura Atom:

```
<title>Example Feed</title>
<link href="http://example.org/"/>
<updated>2003-12-13T18:30:02Z</updated>
<author>
    <name>John Doe</name>
</author>
<id>urn:uuid:60a76c80-d399-11d9-b93C-0003939e0af6</id>

<entry>
    <title>Atom-Powered Robots Run Amok</title>
    <link href="http://example.org/2003/12/13/atom03"/>
    <id>urn:uuid:1225c695-cfb8-4ebb-aaaa-80da344efa6a</id>
    <updated>2003-12-13T18:30:02Z</updated>
    <summary>Some text.</summary>
</entry>
```

## Detaily implementace
- program podporuje:
    - přesměrování
    - RSS/Atom feed
    - segmentované data
- limit na redirect je nastaven na 3
- limit na opakovaný pokus v případě selhání spojení je 3
- limit na selhání spojení se restartuje při každém úspěšném přesměrování

## Tvůrci

[Martin Kocich](https://github.com/KoMatrich) - xkocic02

## Díky

[Vitěslav Kříž](https://github.com/sprtokiller) za řešení kompatabily getopt na FreeBSD a WIN

## Použitá literatura

[Openssl Implementace](https://developer.ibm.com/tutorials/l-openssl/)

[HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)

[Chuked transfer](https://en.wikipedia.org/wiki/Chunked_transfer_encoding)

[Další testovací feedy](https://rss.com/blog/popular-rss-feeds/)

[Příklad RSS](https://www.w3schools.com/XML/xml_rss.asp)

[Příklad Atom](https://www.ietf.org/rfc/rfc4287.txt)

## Licence

Zdrojový kód a dokumentace je pod licencí [MIT](https://mit-license.org/).