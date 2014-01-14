
#include "Voice.h"

bool Phrase_escribible=true;
bool Direction_escribible=true;
bool Words_escribible=true;

///*! \brief Gets the direction of the sound source
// *
// *   Gets the direction of the sound source \return Direction
// */
int Voice::get_Direction() {
    return Direction;
}

///*! \brief Sets the direction of the sound source
// *
// *   Sets the direction of the sound source
// */
void Voice::set_Direction(int value) {
    do
    {
    } while (!Direction_escribible);
    Direction_escribible=false;
    Direction = value;
    Direction_escribible=true;
}

///*! \brief Gets the list of words in the phrase said to the voice recognition system
// *
// *   Gets the list of words in the phrase said to the voice recognition system \return Words
// */
string Voice::get_Words() {
    return *Words;
}

///*! \brief Sets the list of words in the phrase said to the voice recognition system
// *
// *   Sets the list of words in the phrase said to the voice recognition system
// */
void Voice::set_Words(string value) {
    *Words = value;
}

///*! \brief Gets the phrase the said to the voice recognition system
// *
// *   Gets the phrase the user said to the voice recognition system \return Phrase
// */
string Voice::get_Phrase() {
    return *Phrase;
}

///*! \brief Sets the phrase the user said to the voice recognition system
// *
// *   Sets the phrase the user said to the voice recognition system
// */
void Voice::set_Phrase(string value) {
    do
    {
    } while (!Phrase_escribible);
    Phrase_escribible=false;
    *Phrase = value;
    Phrase_escribible=true;
}

