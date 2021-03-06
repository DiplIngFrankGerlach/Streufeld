#ifndef STREUFELD_KETTE
#define STREUFELD_KETTE

 
/* Streufeld mit Verketteter Liste zur Behandlung von Doppelfaellen

   (C) Frank Gerlach 2017

   Nichtkommerzielle Nutzung kostenlos, Kommerzielle Nutzung erfordert eine
   kostenpflichtige Nutzungsvereinbarung. Details bitte unter frankgerlach.tai@gmx.de
   erfragen.
*/
#include "sicherSturz.h"

template<class Schluessel,
         class Wert, 
         class SchluesselAdapter,
         class WertAdapter>
class StreufeldKette
{
    struct Eintrag
    {
       Schluessel m_schluessel;
       Wert       m_wert;
       Eintrag*   m_doppel;
       
       Eintrag():m_doppel(NULL)
       {}
    };

    Eintrag** m_streufeld;
    uint64_t m_groesse;
    uint64_t m_anzahlBelegt;



    void wachse()
    {
        uint64_t neueGroesse;
        if( m_groesse == 0)
        {
            neueGroesse = 1;
        }
        else
        {
           neueGroesse = m_groesse << 1;
        }
        Eintrag** neuFeld = new Eintrag*[neueGroesse];
        if( neuFeld == NULL )
        { 
           sicherSturz("StreufeldKette::wachse()");
        }
        for(uint64_t i=0; i < neueGroesse; i++ )
        {
            neuFeld[i] = NULL;
        }
        for(uint64_t stelle=0; stelle < m_groesse; stelle++ )
        {
            Eintrag* kettenZeiger = m_streufeld[stelle];
            while(kettenZeiger != NULL )
            {
               Eintrag* naechster =  kettenZeiger->m_doppel;
               kettenZeiger->m_doppel = NULL;
               bool schlGef;
               trageEin(neuFeld,neueGroesse,kettenZeiger,schlGef);
               kettenZeiger = naechster;
            }
        }
        delete[] m_streufeld;
        m_streufeld = neuFeld;
        m_groesse = neueGroesse;
    }

    static void trageEin(Eintrag** streuFeld,
                uint64_t neueGroesse,
                Eintrag* einzutragen,
                bool& schluesselGefunden)
    {
         uint64_t randomisiert  = SchluesselAdapter::randomisiere(einzutragen->m_schluessel);
         uint64_t stelle        = randomisiert & (neueGroesse-1);
         Eintrag** kettenZeiger = &streuFeld[stelle];
         while( (*kettenZeiger) != NULL )
         {
            if( SchluesselAdapter::gleich( (*kettenZeiger)->m_schluessel,einzutragen->m_schluessel) )
            {
                WertAdapter::loescheEndgueltig((*kettenZeiger)->m_wert);
                (*kettenZeiger)->m_wert = einzutragen->m_wert;
                SchluesselAdapter::loescheEndgueltig(einzutragen->m_schluessel);
                delete einzutragen;
                schluesselGefunden = true;
                return;
            } 
            kettenZeiger =  &( (*kettenZeiger)->m_doppel );
         }
         *kettenZeiger = einzutragen;         
         schluesselGefunden = false;
    }

public:

    StreufeldKette():m_streufeld(NULL),m_groesse(0),m_anzahlBelegt(0)
    {}

    bool finde(const Schluessel& schluessel, Wert& wert)
    {
         uint64_t randomisiert  = SchluesselAdapter::randomisiere(schluessel);
         uint64_t stelle        = randomisiert & (m_groesse-1);
         Eintrag* kettenZeiger = m_streufeld[stelle];
         while(kettenZeiger != NULL )
         {
            if( SchluesselAdapter::gleich( kettenZeiger->m_schluessel,schluessel) )
            {
                wert =  kettenZeiger->m_wert;
                return true;
            }
            kettenZeiger = kettenZeiger->m_doppel;
         }
         return false;
    }

    void trageEin(const Schluessel& schluessel,const Wert& wert)
    {
       if( m_anzahlBelegt >= (m_groesse >> 1) )
       {
           wachse();
       } 
       Eintrag* eintrag = new Eintrag;
       if( eintrag == NULL ) 
       {
           sicherSturz("StreufeldKette::trageEin()");
       }
       eintrag->m_schluessel = schluessel;
       eintrag->m_wert = wert;
       bool schluesselGefunden;
       trageEin(m_streufeld,m_groesse,eintrag,schluesselGefunden);
       if( !schluesselGefunden )
       {
          m_anzahlBelegt++;
       }
    }

    bool loesche(const Schluessel& schluessel)
    {
         uint64_t randomisiert   = SchluesselAdapter::randomisiere(schluessel);
         uint64_t stelle         = randomisiert & (m_groesse-1);
         Eintrag* kettenZeiger   = m_streufeld[stelle];
         Eintrag* vorigerEintrag = NULL;
         while(kettenZeiger != NULL )
         {
            if( SchluesselAdapter::gleich( kettenZeiger->m_schluessel,schluessel) )
            {
                WertAdapter::loescheEndgueltig(kettenZeiger->m_wert);
                SchluesselAdapter::loescheEndgueltig(kettenZeiger->m_schluessel);
                if( vorigerEintrag == NULL )
                {
                    m_streufeld[stelle] = kettenZeiger->m_doppel;
                }
                else
                {
                   vorigerEintrag->m_doppel = kettenZeiger->m_doppel;
                }
                delete kettenZeiger;
                m_anzahlBelegt--;
                return true;
            }
            vorigerEintrag = kettenZeiger;
            kettenZeiger   = kettenZeiger->m_doppel;
         }
         return false; 
    }

    uint64_t anzahl() const 
    {
       return m_anzahlBelegt;
    } 

    ~StreufeldKette()
    {
        for(uint64_t stelle=0; stelle < m_groesse; stelle++ )
        {
            Eintrag* kettenZeiger = m_streufeld[stelle];
            while(kettenZeiger != NULL )
            {
               Eintrag* naechster =  kettenZeiger->m_doppel;
               WertAdapter::loescheEndgueltig(kettenZeiger->m_wert);
               SchluesselAdapter::loescheEndgueltig(kettenZeiger->m_schluessel); 
               delete kettenZeiger;
               kettenZeiger = naechster;
            }
        }
        delete[] m_streufeld;
        m_groesse = 0;
        m_anzahlBelegt = 0;
    }
};


#endif


