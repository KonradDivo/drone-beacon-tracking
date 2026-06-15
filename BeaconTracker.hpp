#ifndef BEACON_TRACKER_HPP
#define BEACON_TRACKER_HPP

#include <opencv2/opencv.hpp>
#include <vector>

// Structure pour stocker les coordonnées du centre de la cible
struct Point2D {
    int x = -1;
    int y = -1;
};

class BeaconTracker {
private:
    // Seuils de couleur au format HSV (Teinte, Saturation, Valeur)
    cv::Scalar m_lowerHSV;
    cv::Scalar m_upperHSV;
    int m_minArea; // Surface minimale en pixels pour ignorer les parasites

public:
    // Constructeur : configure la couleur de la balise à traquer
    BeaconTracker();

    // Analyse l'image, applique les filtres et extrait les coordonnées de la balise
    Point2D processFrame(cv::Mat& frame);
};

#endif
