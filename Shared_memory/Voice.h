#ifndef _VOICE_H
#define _VOICE_H


#include <string>
using namespace std;

///*! \brief Information about the processed information from the voice recognition
// *
// *
// */
class Voice {
private:
    ///*! \brief String containing the phrase recogniced
    // *
    // *
    // */
    string * Phrase;

    ///*! \brief List of the individual words recognized in the phrase
    // *
    // *
    // */
    string * Words;

    ///*! \brief Value indicating the direction of the sound source
    // *
    // *
    // */
    int Direction;


public:
    ///*! \brief Gets the direction of the sound source
    // *
    // *   Gets the direction of the sound source \return Direction
    // */
    int get_Direction();

    ///*! \brief Sets the direction of the sound source
    // *
    // *   Sets the direction of the sound source
    // */
    void set_Direction(int value);

    ///*! \brief Gets the list of words in the phrase said to the voice recognition system
    // *
    // *   Gets the list of words in the phrase said to the voice recognition system \return Words
    // */
    string get_Words();

    ///*! \brief Sets the list of words in the phrase said to the voice recognition system
    // *
    // *   Sets the list of words in the phrase said to the voice recognition system
    // */
    void set_Words(string value);

    ///*! \brief Gets the phrase the said to the voice recognition system
    // *
    // *   Gets the phrase the user said to the voice recognition system \return Phrase
    // */
    string get_Phrase();

    ///*! \brief Sets the phrase the user said to the voice recognition system
    // *
    // *   Sets the phrase the user said to the voice recognition system
    // */
    void set_Phrase(string value);

    friend class Robot;
};
#endif
