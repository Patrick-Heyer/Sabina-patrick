
#include "Synthesis.h"

bool Current_letter_escribible=true;
bool Speed_escribible=true;
bool Volumen_escribible=true;
bool Phrase_synt_escribible=true;

///*! \brief Gets the letter index the sintetizer is sinthetizing
// *
// *   Gets the letter index the sintetizer is sinthetizing \return Current_letter
// */
int Synthesis::get_Current_letter() {
    return Current_letter;
}

///*! \brief Sets the letter index the sintetizer is sinthetizing
// *
// *   Sets the letter index the sintetizer is sinthetizing
// */
void Synthesis::set_Current_letter(int value) {
    do
    {
    } while (!Current_letter_escribible);
    Current_letter_escribible=false;
    Current_letter = value;
    Current_letter_escribible=true;
}

///*! \brief Gets the speed at which the sintetizer is sinthetizing
// *
// *   Gets the speed at which the sintetizer is sinthetizing \return Speed
// */
float Synthesis::get_Speed() {
    return Speed;
}

///*! \brief Sets the speed at which the sintetizer is sinthetizing
// *
// *   Sets the speed at which the sintetizer is sinthetizing
// */
void Synthesis::set_Speed(float value) {
    do
    {
    } while (!Speed_escribible);
    Speed_escribible=false;
    Speed = value;
    Speed_escribible=true;
}

///*! \brief Gets the volumen at which the sintetizer is sinthetizing
// *
// *   Gets the volumen at which the sintetizer is sinthetizing \return Volumen
// */
int Synthesis::get_Volumen() {
    return Volumen;
}

///*! \brief Sets the volumen at which the sintetizer is sinthetizing
// *
// *   Sets the volumen at which the sintetizer is sinthetizing
// */
void Synthesis::set_Volumen(int value) {
    do
    {
    } while (!Volumen_escribible);
    Volumen_escribible=false;
    Volumen = value;
    Volumen_escribible=true;
}

///*! \brief Gets the phrase the sintetizer will say
// *
// *   Gets the phrase the sintetizer will say \return Phrase
// */
string Synthesis::get_Phrase() {
    return *Phrase;
}

///*! \brief Sets the phrase the sintetizer will say
// *
// *   Sets the phrase the sintetizer will say
// */
void Synthesis::set_Phrase(string value) {
    do
    {
    } while (!Phrase_synt_escribible);
    Phrase_synt_escribible=false;
    *Phrase = value;
    Phrase_synt_escribible=true;

}


