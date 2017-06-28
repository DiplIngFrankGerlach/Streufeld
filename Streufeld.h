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

#ifndef SUCHFELD
#define SUCHFELD

#include <iostream>

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
   uint64_t m_anzahlBesetzt;
   Eintrag* m_eintraege;
public:
   Streufeld(uint64_t kapazitaet,bool& erfolg):m_kapazitaet(0),
                                              m_anzahlBesetzt(0)
   {
     m_eintraege = new Eintrag[kapazitaet];
     if( m_eintraege == NULL )
     {
       erfolg = false;
       return;
     } 
     uint64_t besetztFeldLaenge = (kapazitaet >> 3) + 1;
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
     erfolg = true;
   }

private:
   bool besetzt(uint64_t stelle)
   {
     return m_besetzt[stelle >> 3] & ( 1 << (stelle & 7) );
   }

   /* versuche den Eintrag mit dem gegebenen Schluessel zu finden.
      Im Erfolgsfall wird "wert" gesetzt und true zurueckgegeben.
      Falls der Eintrag nicht existiert, wird nur false zurueckgegeben
   */
   bool findeIntern( const Schluessel& schluessel, 
                     uint64_t &stelle, 
                     Wert& wert)
   {
      cout << "findeIntern: m_anzahlBesetzt:" << m_anzahlBesetzt << " schluessel:" << schluessel << 
              " stelle:" << stelle << endl;
      uint64_t stelleKopie = stelle;
      if( m_anzahlBesetzt > 0)
      {
         while( besetzt(stelle) )
         {
            cout << "stelle:" << stelle << endl;
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

public:
   bool finde(const Schluessel& schluessel,
              Wert& wert) 
   {
      uint64_t randomisiert = SchluesselAdapter::randomisiere(schluessel);
      uint64_t stelle = randomisiert % m_kapazitaet;
      return findeIntern(schluessel,stelle, wert); 
   }

   bool trageEin(const Schluessel& schluessel, const Wert& wert)
   {
      uint64_t randomisiert = SchluesselAdapter::randomisiere(schluessel);   
      uint64_t stelle = randomisiert % m_kapazitaet;
      Wert altWert;
      if( findeIntern(schluessel, stelle, altWert) )
      {
         WertAdapter::loesche(m_eintraege[stelle].m_wert);
         m_eintraege[stelle].m_wert = wert; 
      }
      else
      { 
         if( float(m_kapazitaet *0.80f) < (m_anzahlBesetzt+1) )
         {
             bool erfolg; 
             Streufeld* suchfeldGroesser = 
                 new Streufeld<Schluessel,Wert,SchluesselAdapter,WertAdapter>( (m_kapazitaet+1)*2, erfolg);
             if( suchfeldGroesser == NULL )
             {
               return false;
             }
             for(uint64_t stelle=0; stelle < m_kapazitaet; stelle++)
             {
                if( besetzt(stelle) )
                {
                   suchfeldGroesser->trageEin(m_eintraege[stelle].m_schluessel,
                                              m_eintraege[stelle].m_wert);
                   WertAdapter::loesche(m_eintraege[stelle].m_wert);
                   SchluesselAdapter::loesche(m_eintraege[stelle].m_schluessel);
                } 
             }
             delete[] m_eintraege;
             m_eintraege = suchfeldGroesser->m_eintraege;
             suchfeldGroesser->m_eintraege = NULL;
             m_kapazitaet = suchfeldGroesser->m_kapazitaet;
             delete[] m_besetzt;
             m_besetzt = suchfeldGroesser->m_besetzt;
             suchfeldGroesser->m_besetzt = NULL;

             delete suchfeldGroesser;

             randomisiert = SchluesselAdapter::randomisiere(schluessel);
             stelle = randomisiert % m_kapazitaet;
         }
         while( besetzt(stelle) )
         {
            stelle++; 
            if( stelle== m_kapazitaet )
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
       uint64_t stelle;
       if( findeIntern(schluessel,stelle,wert) )
       {
          SchluesselAdapter::loesche(m_eintraege[stelle].m_schluessel);
          WertAdapter::loesche(m_eintraege[stelle].m_wert);
          char inversMaske = 0xFF ^ (1 << (stelle & 7));
          m_besetzt[stelle >> 3] &= inversMaske;       
          m_anzahlBesetzt--;
          return true;
       }
       return false;
   } 
   ~Streufeld()
   {
       delete[] m_eintraege;
       delete[] m_besetzt;
   } 
};

#endif
