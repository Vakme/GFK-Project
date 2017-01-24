#include "comparator.h"
#include "element.h"
#include "utils.h"
#include <memory>
#include <cmath>

void Comparator::compareElems(utils::unique_vector<Element> & elementsOnPanel, utils::unique_vector<Element> & elementsOnCanvas) {

    bool areEqual = false;
    if(elementsOnCanvas.size() == elementsOnPanel.size()) {
        qDebug() << "Ten sam rozmiar, idziemy dalej";
        std::sort(elementsOnCanvas.begin(), elementsOnCanvas.end(), [] (
                  std::unique_ptr<Element> const& a, std::unique_ptr<Element> const& b) { return static_cast<int>(a->typ) < static_cast<int>(b->typ); });
        std::sort(elementsOnPanel.begin(), elementsOnPanel.end(), [] (
                  std::unique_ptr<Element> const& a, std::unique_ptr<Element> const& b) { return static_cast<int>(a->typ) < static_cast<int>(b->typ); });

        qDebug() << "Już po";
        for(auto & el : elementsOnPanel)
            qDebug() << static_cast<int>(el -> typ);

        areEqual = true;
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
                if(i+1 < elementsOnCanvas.size() && elementsOnCanvas.at(i)->typ == elementsOnCanvas.at(i+1)->typ) {
                    cRot += elementsOnCanvas.at(i+1)->getRot() + elC.get()->getRot();
                    pRot += elementsOnPanel.at(i+1)->getRot() + elC.get()->getRot();
                    i++;
                }
                if(fabs(cRot - pRot) > 5) {
                    qDebug() << "Zły kąt względny";
                    areEqual = false;
                }

            }
            for(int i=1; i < elementsOnPanel.size(); i++) {
                if(i+1 < elementsOnPanel.size() && elementsOnPanel.at(i)->typ == elementsOnPanel.at(i+1)->typ) {
                    elPDiffs.push_back(fabs(elP->centerPoint().manhattanLength()
                                            - elementsOnPanel.at(i)->centerPoint().manhattanLength())
                                       + fabs(elP->centerPoint().manhattanLength()
                                              - elementsOnPanel.at(i+1)->centerPoint().manhattanLength()));
                    i++;
                }
                else
                    elPDiffs.push_back(fabs(elC->centerPoint().manhattanLength() - elementsOnPanel.at(i)->centerPoint().manhattanLength()));
            }

            for(int i=1; i < elementsOnCanvas.size(); i++) {
                if(i+1 < elementsOnCanvas.size() && elementsOnCanvas.at(i)->typ == elementsOnCanvas.at(i+1)->typ) {
                    elCDiffs.push_back(fabs(elC->centerPoint().manhattanLength()
                                            - elementsOnCanvas.at(i)->centerPoint().manhattanLength())
                                       + fabs(elC->centerPoint().manhattanLength()
                                              - elementsOnCanvas.at(i+1)->centerPoint().manhattanLength()));
                    i++;
                }
                else
                    elCDiffs.push_back(fabs(elC->centerPoint().manhattanLength() - elementsOnCanvas.at(i)->centerPoint().manhattanLength()));
            }
            for(int i = 0; i < elCDiffs.size(); i++) {
                if(fabs(elCDiffs.at(i) - elPDiffs.at(i)) > 5) {
                    qDebug() << "Elementy położone inaczej względem siebie";
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
    if(fabs(cElement->getRot()-pElement->getRot()) > 30) {
        qDebug() << "Zły kąt";
        return false ;
    }
    if(fabs(cElement->centerPoint().manhattanLength()-pElement->centerPoint().manhattanLength()) > 150) {
        qDebug() << "Prawdopodobnie złe położenie";
        return false;
    }
    qDebug() << "Pierwsze położenie dobre";
    return true;
}

