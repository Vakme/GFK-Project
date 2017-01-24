#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "utils.h"
#include "element.h"

class Comparator
{
public:
    Comparator() = delete;
    Comparator(const Comparator &) = delete;
    static bool compareElement(Element * cElement, Element * pElement);
    static void compareElems(utils::unique_vector<Element> & elementsOnPanel, utils::unique_vector<Element> & elementsOnCanvas);
};

#endif // COMPARATOR_H
