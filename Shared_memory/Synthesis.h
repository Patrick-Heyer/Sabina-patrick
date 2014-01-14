#ifndef _SYNTHESIS_H
#define _SYNTHESIS_H


#include <string>
using namespace std;

///*! \brief  Information regarding the voice synthesis
// *
// *
// */
class Synthesis {
private:
    ///*! \brief String containing the phrase that will be synthesized
    // *
    // *
    // */
    string * Phrase;

    ///*! \brief Letter currently being synthesized
    // *
    // *
    // */
    int Current_letter;

    ///*! \brief Speed of the synthetization
    // *
    // *
    // */
    float Speed;

    ///*! \brief Volume the synthezicer should use for this phrase
    // *
    // *
    // */
    int Volumen;


public:
    ///*! \brief Gets the letter index the sintetizer is sinthetizing
    // *
    // *   Gets the letter index the sintetizer is sinthetizing \return Current_letter
    // */
    int get_Current_letter();

    ///*! \brief Sets the letter index the sintetizer is sinthetizing
    // *
    // *   Sets the letter index the sintetizer is sinthetizing
    // */
    void set_Current_letter(int value);

    ///*! \brief Gets the speed at which the sintetizer is sinthetizing
    // *
    // *   Gets the speed at which the sintetizer is sinthetizing \return Speed
    // */
    float get_Speed();

    ///*! \brief Sets the speed at which the sintetizer is sinthetizing
    // *
    // *   Sets the speed at which the sintetizer is sinthetizing
    // */
    void set_Speed(float value);

    ///*! \brief Gets the volumen at which the sintetizer is sinthetizing
    // *
    // *   Gets the volumen at which the sintetizer is sinthetizing \return Volumen
    // */
    int get_Volumen();

    ///*! \brief Sets the volumen at which the sintetizer is sinthetizing
    // *
    // *   Sets the volumen at which the sintetizer is sinthetizing
    // */
    void set_Volumen(int value);

    ///*! \brief Gets the phrase the sintetizer will say
    // *
    // *   Gets the phrase the sintetizer will say \return Phrase
    // */
    string get_Phrase();

    ///*! \brief Sets the phrase the sintetizer will say
    // *
    // *   Sets the phrase the sintetizer will say
    // */
    void set_Phrase(string value);

    friend class Robot;
};
#endif
