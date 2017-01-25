#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "utils.h"
#include "element.h"

/*! \class  Comparator
 *  \brief  Static class comparing elements on Panel and on Canvas
 */
class Comparator
{
public:

    /*!
     * \fn      Comparator
     * \brief   Constructor not needed for a static class
     */
    Comparator() = delete;

    /*!
     * \fn      Comparator
     * \brief   Copy constructor not needed for a static class
     * \param   comparator  Object to copy
     */
    Comparator(const Comparator &) = delete;

    /*!
     * \fn      compareElement
     * \brief   Compares a single element with highest tolerance
     * \param   cElement  Element displayed on canvas to compare
     * \param   pElement  Element displayed on panel to compare
     * \return  true if elements are equal (similar), false if not
     */
    static bool compareElement(Element * cElement, Element * pElement);

    /*!
     * \fn      compareElement
     * \brief   Compares user puzzles with image on panel
     * \param   elementsOnCanvas  Vector of elements displayed on canvas to compare
     * \param   elementsOnPanel   Vector of elements displayed on panel to compare
     */
    static void compareElems(utils::unique_vector<Element> & elementsOnPanel,
                             utils::unique_vector<Element> & elementsOnCanvas);
};

#endif // COMPARATOR_H
