#include "comparator.h"
#include "element.h"
#include "utils.h"
#include <memory>
#include <cmath>

void Comparator::compare(utils::unique_vector<Element> & elementsOnPanel, utils::unique_vector<Element> & elementsOnCanvas) {

    bool areEqual = false;
    if(elementsOnCanvas.size() == elementsOnPanel.size()) {
        areEqual = true;
        //qDebug() << "Ten sam rozmiar, idziemy dalej";
        std::unique_ptr<Element> & elC = elementsOnCanvas.at(0), & elP = elementsOnPanel.at(0);
        //qDebug() << "Jeszcze działam";
        /*for(std::unique_ptr<Element> & pElement : elementsOnPanel) {
            if(pElement->typ == elC->typ) {
                elP = std::move(pElement);
                break;
            }
        }
        qDebug() << "Jeszcze działam";*/
        areEqual = compareElement(elC.get(), elP.get());
        if(areEqual) {
            QVector<qreal> elCDiffs, elPDiffs;
            qreal cRot, pRot;
            for(int i=1; i < elementsOnCanvas.size(); i++) {
                cRot = elementsOnCanvas.at(i)->getRot() + elC.get()->getRot();
                pRot = elementsOnPanel.at(i)->getRot() + elC.get()->getRot();
                if(fabs(cRot - pRot) > 5) {
                    //qDebug() << "Zły kąt względny";
                    areEqual = false;
                }

            }
            for(auto & pEl : elementsOnPanel)
                if(pEl != elP)
                    elPDiffs.push_back(fabs(elP->centerPoint.manhattanLength() - pEl->centerPoint.manhattanLength()));
            for(auto & cEl : elementsOnCanvas)
                if(cEl != elC)
                    elCDiffs.push_back(fabs(elC->centerPoint.manhattanLength() - cEl->centerPoint.manhattanLength()));
            for(int i = 0; i < elCDiffs.size(); i++) {
                if(fabs(elCDiffs.at(i) - elPDiffs.at(i)) > 5) {
                    //qDebug() << "Elementy położone inaczej względem siebie";
                    areEqual = false;
                }
            }
        }
    }
    if(areEqual)
        qDebug() << "BRAWO, UŁOŻYŁEŚ!!!";
    else
        qDebug() << "NOPE, NIE TAK";


}


bool Comparator::compareElement(Element * cElement, Element * pElement) {
    qDebug() << "Jeszcze wewnątrz: " << static_cast<int>(cElement->typ) << " " << static_cast<int>(pElement->typ);
    if(cElement->getMir()!=pElement->getMir()) {
        qDebug() << "Złe odbicie";
        return false;
    }
    if(fabs(cElement->getRot()-pElement->getRot()) > 10) {
        qDebug() << "Zły kąt";
        return false ;
    }
    if(fabs(cElement->centerPoint.manhattanLength()-pElement->centerPoint.manhattanLength()) > 50) {
        qDebug() << "Prawdopodobnie złe położenie";
        return false;
    }
    qDebug() << "Pierwsze położenie dobre";
    return true;
}

