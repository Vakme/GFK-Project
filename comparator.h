/*! \file  comparator.h
    \brief Contains comparator class.
*/

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "utils.h"
#include "element.h"

//! Static class comparing elements on Panel and on Canvas
class Comparator {
public:
    //! Constructor is not needed for a static class
    Comparator() = delete;

    //! Copy constructor is not needed for a static class
    /*!
      \param   comp  instance to make a copy of
     */
    Comparator(const Comparator & comp) = delete;

    //! Compares a single element with highest tolerance
    /*!
      \param   cElement  puzzle piece displayed on canvas to compare
      \param   pElement  puzzle piece displayed on panel to compare
      \return  true if elements are equal (similar), false if not
     */
    static bool compareElement(Element * cElement, Element * pElement);

    //! Compares user puzzles with image on panel
    /*!
      \param   elementsOnCanvas  vector of puzzle pieces displayed on canvas to compare
      \param   elementsOnPanel   vector of puzzle pieces displayed on panel to compare
     */
    static void compareElems(utils::unique_vector<Element> & elementsOnPanel,
                             utils::unique_vector<Element> & elementsOnCanvas);
};

#endif // COMPARATOR_H
