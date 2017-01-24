#include "comparator.h"
#include "element.h"
#include "utils.h"
#include <memory>
#include <cmath>
#include <QMessageBox>

void Comparator::compareElems(utils::unique_vector<Element> & elementsOnPanel, utils::unique_vector<Element> & elementsOnCanvas) {

    bool areEqual = false;
    if(elementsOnCanvas.size() == elementsOnPanel.size()) {
#if DEBUG
        qDebug() << "Ten sam rozmiar, idziemy dalej";
#endif
        std::sort(elementsOnCanvas.begin(), elementsOnCanvas.end(), [] (
                  std::unique_ptr<Element> const& a, std::unique_ptr<Element> const& b) { return static_cast<int>(a->typ) < static_cast<int>(b->typ); });
        std::sort(elementsOnPanel.begin(), elementsOnPanel.end(), [] (
                  std::unique_ptr<Element> const& a, std::unique_ptr<Element> const& b) { return static_cast<int>(a->typ) < static_cast<int>(b->typ); });

        areEqual = true;
        std::unique_ptr<Element> & elC = elementsOnCanvas.at(0), & elP = elementsOnPanel.at(0);
#if DEBUG
        qDebug() << "Jeszcze działam";
#endif
        areEqual = compareElement(elC.get(), elP.get());
        if(areEqual) {
            QVector<qreal> elCDiffs, elPDiffs;
            qreal cRot, pRot;
            for(unsigned int i=1; i < elementsOnCanvas.size(); i++) {
                cRot = elementsOnCanvas.at(i)->getRot() + elC.get()->getRot();
                pRot = elementsOnPanel.at(i)->getRot() + elC.get()->getRot();
                if(i+1 < elementsOnCanvas.size() && elementsOnCanvas.at(i)->typ == elementsOnCanvas.at(i+1)->typ) {
                    cRot += elementsOnCanvas.at(i+1)->getRot() + elC.get()->getRot();
                    pRot += elementsOnPanel.at(i+1)->getRot() + elC.get()->getRot();
                    i++;
                }
                if(fabs(cRot - pRot) > 5) {
#if DEBUG
                    qDebug() << "Zły kąt względny";
#endif
                    areEqual = false;
                }

            }
            for(unsigned int i=1; i < elementsOnPanel.size(); i++) {
                if(i+1 < elementsOnPanel.size() && elementsOnPanel.at(i)->typ == elementsOnPanel.at(i+1)->typ) {
                    elPDiffs.push_back(fabs(elP->centerPoint().manhattanLength()
                                            - elementsOnPanel.at(i)->centerPoint().manhattanLength())
                                       + fabs(elP->centerPoint().manhattanLength()
                                              - elementsOnPanel.at(i+1)->centerPoint().manhattanLength()));
                    i++;
                }
                else
                    elPDiffs.push_back(fabs(elP->centerPoint().manhattanLength() - elementsOnPanel.at(i)->centerPoint().manhattanLength()));
            }

            for(unsigned int i=1; i < elementsOnCanvas.size(); i++) {
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
                if(fabs(elCDiffs.at(i) - elPDiffs.at(i)) > 10) {
#if DEBUG
                    qDebug() << "Elementy położone inaczej względem siebie" << i << " " << elCDiffs.at(i) << " " << elPDiffs.at(i);
#endif
                    areEqual = false;
                }
            }
        }
    }
    if(areEqual)
        QMessageBox(QMessageBox::NoIcon, "Sukces!", "Brawo, udało ci się ułożyć!").exec();


}


bool Comparator::compareElement(Element * cElement, Element * pElement) {
#if DEBUG
    qDebug() << "Jeszcze wewnątrz: " << static_cast<int>(cElement->typ) << " " << static_cast<int>(pElement->typ);
#endif
    if(cElement->getMir()!=pElement->getMir()) {
#if DEBUG
        qDebug() << "Złe odbicie";
#endif
        return false;
    }
    if(fabs(cElement->getRot()-pElement->getRot()) > 30) {
#if DEBUG
        qDebug() << "Zły kąt";
#endif
        return false ;
    }
    if(fabs(cElement->centerPoint().manhattanLength()-pElement->centerPoint().manhattanLength()) > 150) {
#if DEBUG
        qDebug() << "Prawdopodobnie złe położenie";
#endif
        return false;
    }
#if DEBUG
    qDebug() << "Pierwsze położenie dobre";
#endif
    return true;
}

