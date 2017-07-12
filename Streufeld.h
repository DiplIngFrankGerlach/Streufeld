/* Effizientes Streufeld (engl. "Hashtable").

   Besonders optimiert auf Speichereffizienz und Anpassbarkeit
   an spezielle Schluessel und Wert-Datenstrukturen.

   Durch Adapterklasse an beliebige Schluessel- und Wert-
   Datenstrukturen anpassbar.

   (C) Frank Gerlach 2017

   Nichtkommerzielle Nutzung ist lizenz- und kostenfrei.

   Kommerzielle Nutzung erfordert eine kostenpflichtige Lizenz.
   Nutzungsplaene fangen an ab 150 Euro pro Produkt.
 
   Einzelheiten bitte erfragen bei frankgerlach.tai@gmx.de

*/

#ifndef STREUFELD
#define STREUFELD

#include <iostream>
#include <stdint.h>
#include "Nuetzlich.h"

using namespace std;

template<class Schluessel,  
         class Wert, 
         class SchluesselAdapter, 
         class WertAdapter>

class Streufeld
{
   struct Eintrag
   {
      Schluessel m_schluessel;
      Wert       m_wert;
   };

   uint8_t* m_besetzt;//bitfeld, das Besetzung einer Stelle anzeigt.
   uint64_t m_kapazitaet;
   uint64_t m_neuHashSchwelle;
   uint64_t m_anzahlBesetzt;
   Eintrag* m_eintraege;
public:
   Streufeld(uint64_t kapazitaet,bool& erfolg):m_besetzt(NULL),
                                               m_kapazitaet(0),
                                               m_neuHashSchwelle(0),
                                               m_anzahlBesetzt(0),                                               
                                               m_eintraege(NULL)
   {
     if( kapazitaet < 2)
     {
         kapazitaet = 2;
     }
     kapazitaet = naechstGroessereZweierPotenz(kapazitaet);

     m_eintraege = new Eintrag[kapazitaet];
     if( m_eintraege == NULL )
     {
       erfolg = false;
       return;
     } 
     uint64_t besetztFeldLaenge = kapazitaet >> 3;
     if( besetztFeldLaenge == 0 )
     {
        besetztFeldLaenge = 1;
     }
     m_besetzt   = new uint8_t[besetztFeldLaenge];
     if( m_besetzt == NULL )
     {
       delete[] m_eintraege;
       m_eintraege = NULL;
       erfolg = false;
       return;
     } 
     for(uint64_t i=0; i < besetztFeldLaenge; i++)
     {
       m_besetzt[i] = 0;
     }
     m_kapazitaet = kapazitaet;
     m_neuHashSchwelle = uint64_t(float(m_kapazitaet)*0.7f);
     erfolg = true;
   }

   

private:



   bool besetzt(uint64_t stelle)
   {
     return m_besetzt[stelle >> 3] & ( 1 << (stelle & 7) );
   }

   void loescheInMaske(uint64_t stelle)
   {
      uint8_t inversMaske = 0xFF ^ (1 << (stelle & 7));
      m_besetzt[stelle >> 3] &= inversMaske;
   }

   /* versuche den Eintrag mit dem gegebenen Schluessel zu finden.
      Im Erfolgsfall wird "wert" gesetzt und true zurueckgegeben.
      Falls der Eintrag nicht existiert, wird nur false zurueckgegeben
   */
   bool findeIntern( const Schluessel& schluessel, 
                     uint64_t &stelle, 
                     Wert& wert)
   {
      //cout << "findeIntern: m_anzahlBesetzt:" << m_anzahlBesetzt << " schluessel:" << schluessel << 
      //        " stelle:" << stelle << endl;
      uint64_t stelleKopie = stelle;
      if( m_anzahlBesetzt > 0)
      {
         while( besetzt(stelle) )
         {
            //cout << "stelle:" << stelle << endl;
            if( SchluesselAdapter::gleich(m_eintraege[stelle].m_schluessel, schluessel) )
            {
               wert = m_eintraege[stelle].m_wert;
               return true;
            }
            stelle++;
            if( stelle == m_kapazitaet )
            {
              stelle = 0;
            }
         }
      }
      stelle = stelleKopie;
      return false;
   }

   /* verdopple die Kapazitaet der Tabelle */
   bool wachse()
   {
       bool erfolg; 
       uint64_t neuKapazitaet = m_kapazitaet << 1;

       if(neuKapazitaet < 2 )
       {
          neuKapazitaet  = 2;
       }
       if( neuKapazitaet < m_kapazitaet )
       {
          return false;
       }
       Streufeld* suchfeldGroesser = 
                    new Streufeld<Schluessel,Wert,SchluesselAdapter,WertAdapter>( neuKapazitaet, erfolg);
       if( !erfolg || (suchfeldGroesser == NULL) )
       {
         return false;
       }
       for(uint64_t stelle2=0; stelle2 < m_kapazitaet; stelle2++)
       {
          if( besetzt(stelle2) )
          {
             if( !suchfeldGroesser->trageEin(m_eintraege[stelle2].m_schluessel,
                                             m_eintraege[stelle2].m_wert) )
             {
                 return false;
             }
             WertAdapter::loesche(m_eintraege[stelle2].m_wert);
             SchluesselAdapter::loesche(m_eintraege[stelle2].m_schluessel);
          } 
       }
       delete[] m_eintraege;
       m_eintraege = suchfeldGroesser->m_eintraege;
       suchfeldGroesser->m_eintraege = NULL;
       m_kapazitaet = suchfeldGroesser->m_kapazitaet;
       m_neuHashSchwelle = suchfeldGroesser->m_neuHashSchwelle;
       delete[] m_besetzt;
       m_besetzt = suchfeldGroesser->m_besetzt;
       suchfeldGroesser->m_besetzt = NULL;

       suchfeldGroesser->m_kapazitaet = 0;
       suchfeldGroesser->m_anzahlBesetzt = 0;
       delete suchfeldGroesser;
       return true;
   }

   void loescheFeldEndgueltig()
   {

       for(uint64_t stelle=0; stelle < m_kapazitaet; stelle++)
       {
          if( besetzt(stelle) )
          {
              SchluesselAdapter::loescheEndgueltig(m_eintraege[stelle].m_schluessel);
              WertAdapter::loescheEndgueltig(m_eintraege[stelle].m_wert);
              loescheInMaske(stelle); 
          }
       }
       
       delete[] m_eintraege;
       m_eintraege = NULL;
       delete[] m_besetzt;
       m_besetzt = NULL;
       m_kapazitaet = 0;
       m_anzahlBesetzt = 0;
 
   }

public:
   bool finde(const Schluessel& schluessel,
              Wert& wert) 
   {
      uint64_t randomisiert = SchluesselAdapter::randomisiere(schluessel);
      uint64_t stelle = randomisiert & (m_kapazitaet-1);
      return findeIntern(schluessel,stelle, wert); 
   }

   bool trageEin(const Schluessel& schluessel, const Wert& wert)
   {
      
      uint64_t randomisiert = SchluesselAdapter::randomisiere(schluessel);   
      uint64_t stelle = randomisiert & (m_kapazitaet-1);
      Wert altWert;
      if( findeIntern(schluessel, stelle, altWert) )
      {
         WertAdapter::loesche(m_eintraege[stelle].m_wert);
         m_eintraege[stelle].m_wert = wert; 
      }
      else
      { 
         if( m_neuHashSchwelle == m_anzahlBesetzt  )
         {
             wachse();
             randomisiert = SchluesselAdapter::randomisiere(schluessel);
         }         
         stelle = randomisiert & (m_kapazitaet-1);
         while( besetzt(stelle) )
         {
            stelle++; 
            if( stelle == m_kapazitaet )
            {
              stelle= 0;
            }
         } 
         m_eintraege[stelle].m_schluessel = schluessel;
         m_eintraege[stelle].m_wert = wert;
         m_besetzt[stelle >> 3] |= (1 << (stelle & 7)); 
         m_anzahlBesetzt++;
      }
      return true;
   } 
   
   
   bool loesche(const Schluessel& schluessel)
   {
       Wert wert;
       uint64_t randomisiert = SchluesselAdapter::randomisiere(schluessel);   
       uint64_t stelle = randomisiert & (m_kapazitaet-1);
       if( findeIntern(schluessel,stelle,wert) )
       {
          SchluesselAdapter::loescheEndgueltig(m_eintraege[stelle].m_schluessel);
          WertAdapter::loescheEndgueltig(m_eintraege[stelle].m_wert);
          loescheInMaske(stelle);       
          m_anzahlBesetzt--;

          //nun muessen noch moeglicherweise "verdraengte" Eintraege "nach vorne geholt" werden
          Schluessel* verdraengtSchluessel;
          Wert* verdraengtWert;
          
          uint64_t anzahlZuSchieben(0); 
          uint64_t naechste = stelle+1;
          do
          {
              if( naechste == m_kapazitaet )
              {
                  naechste = 0;
              }
              if( besetzt(naechste) )
              {
                 randomisiert = SchluesselAdapter::randomisiere(m_eintraege[naechste].m_schluessel);
                 uint64_t eigentlicheStelle = randomisiert & (m_kapazitaet-1);
                 if( eigentlicheStelle != naechste )
                 {
                     anzahlZuSchieben++;                           
                 }
              }
              else break;
              naechste++;
          }
          while(true);
          verdraengtSchluessel = new Schluessel[anzahlZuSchieben];
          verdraengtWert       = new Wert[anzahlZuSchieben];
          if( (verdraengtSchluessel == NULL) ||  (verdraengtWert == NULL))
          {
             return false;
          }

          naechste = stelle+1;
          uint64_t verdraengtZaehler(0);
          do
          {
              if( naechste == m_kapazitaet )
              {
                  naechste = 0;
              }
              if( besetzt(naechste) )
              {
                 randomisiert = SchluesselAdapter::randomisiere(m_eintraege[naechste].m_schluessel);
                 uint64_t eigentlicheStelle = randomisiert & (m_kapazitaet-1);
                 if( eigentlicheStelle != naechste )
                 {
                     verdraengtSchluessel[verdraengtZaehler] = m_eintraege[naechste].m_schluessel;
                     SchluesselAdapter::loesche(m_eintraege[naechste].m_schluessel);
                     verdraengtWert[verdraengtZaehler++] = m_eintraege[naechste].m_wert;
                     WertAdapter::loesche(m_eintraege[naechste].m_wert);
                     loescheInMaske(naechste);  
                     m_anzahlBesetzt--;                          
                 }
              }
              else break;
              naechste++;
          }
          while(true);
          for(uint64_t i=0; i < verdraengtZaehler; i++)
          {
             trageEin(verdraengtSchluessel[i],verdraengtWert[i]);
             SchluesselAdapter::loesche(verdraengtSchluessel[i]);
             WertAdapter::loesche(verdraengtWert[i]);
          }
          delete[] verdraengtSchluessel;
          delete[] verdraengtWert;
          return true;
       }
       return false;
   }
    
   uint64_t anzahl() const 
   {
      return m_anzahlBesetzt;
   } 

   void loescheFeld()
   {
       for(uint64_t stelle=0; stelle < m_kapazitaet; stelle++)
       {
          if( besetzt(stelle) )
          {
              SchluesselAdapter::loesche(m_eintraege[stelle].m_schluessel);
              WertAdapter::loesche(m_eintraege[stelle].m_wert);
              loescheInMaske(stelle); 
          }
       }
   }

   

   ~Streufeld()
   {
       loescheFeldEndgueltig();
   } 


};

#endif
